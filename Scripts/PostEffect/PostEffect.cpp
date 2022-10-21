#include"PostEffect.h"
#include"d3dx12.h"
#include "../WindowsAPI/WinAPI.h"
#include <d3dcompiler.h>
#include "../BaseDirectX/Input.h"
#include "../imgui/ImguiControl.h"
#include "../Camera/Camera.h"
#pragma comment (lib, "d3dcompiler.lib")

using namespace DirectX;
UINT PostEffect::frameTime;
float PostEffect::effectType;

PostEffect PostEffects::postWater;
PostEffect PostEffects::postMosaic;
PostEffect PostEffects::postBlur;
PostEffect PostEffects::postNormal;
PostEffects::PostEffectType PostEffects::type;

PostEffect::PostEffect()
{
	frameTime = 0;
}

void PostEffect::Initialize(HLSLShader& shader)
{
	HRESULT result;
	VertexPosUv Spritevertices[] = {
		{{ -1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
		{{ -1.0f, +1.0f, 0.0f}, {0.0f, 0.0f}},
		{{ +1.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
		{{ +1.0f, +1.0f, 0.0f}, {1.0f, 0.0f}},
	};
	CreateGraphicsPipelineState(shader);

	int sizevb = sizeof(Spritevertices);

	//���_�}�b�v
	CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizevb);
	result = BaseDirectX::GetInstance()->dev->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff));
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, Spritevertices, sizeof(Spritevertices));
	vertBuff->Unmap(0, nullptr);
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(Spritevertices);
	vbView.StrideInBytes = sizeof(Spritevertices[0]);
	CD3DX12_RESOURCE_DESC resourceDescConst = CD3DX12_RESOURCE_DESC::Buffer((sizeof(PostEffectConstBuffer) + 0xff) & ~0xff);
	result = BaseDirectX::GetInstance()->dev->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDescConst, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constBuff));

	//�K�E�X�u���[�p
	CalcWeightGaussian(weights, NumWeight, 8.0f);
	XMFLOAT4 weight0 = { weights[0], weights[1] ,weights[2] ,weights[3] };
	XMFLOAT4 weight1 = { weights[4], weights[5] ,weights[6] ,weights[7] };
	//�萔�}�b�v
	PostEffectConstBuffer* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = XMMatrixIdentity();
	constMap->weight0 = weight0;
	constMap->weight1 = weight1;
	constMap->frameTime = frameTime;
	constBuff->Unmap(0, nullptr);

	//�e�N�X�`���o�b�t�@�̍쐬
	renderTarget.TexInit();
	//SRV�p�f�X�N���v�^�q�[�v
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = PostEffect::texNum;//mulchTerxter�p��2�ɕύX

	result = BaseDirectX::GetInstance()->dev->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));
	//SRV
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	for (int i = 0; i < PostEffect::texNum; i++)
	{
		BaseDirectX::GetInstance()->dev->CreateShaderResourceView(renderTarget.texBuff[i].Get(), &srvDesc, CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapSRV->GetCPUDescriptorHandleForHeapStart(), i, BaseDirectX::GetInstance()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	}

	renderTarget.RTVInit();
	//�[�x�o�b�t�@
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, WindowsAPI::window_width, WindowsAPI::window_height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
	CD3DX12_HEAP_PROPERTIES heapPropDef(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_CLEAR_VALUE clearValue(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
	result = BaseDirectX::GetInstance()->dev->CreateCommittedResource(&heapPropDef, D3D12_HEAP_FLAG_NONE, &depthResDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &clearValue, IID_PPV_ARGS(&depthBuff));
	assert(SUCCEEDED(result));
	//DSV
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = RenderTarget::renderNum;
	result = BaseDirectX::GetInstance()->dev->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	BaseDirectX::GetInstance()->dev->CreateDepthStencilView(depthBuff.Get(), &dsvDesc, descHeapDSV->GetCPUDescriptorHandleForHeapStart());
}

void PostEffect::Draw()
{
	/*if (Input::KeyTrigger(DIK_0))
	{
		static int tex = 0;
		tex = (tex + 1) % 2;
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		baseDirectX.dev->CreateShaderResourceView(renderTarget.texBuff[tex].Get(), &srvDesc, descHeapSRV->GetCPUDescriptorHandleForHeapStart());
	}*/
	XMMATRIX matScale, matRot, matTrans;
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(pos.x, pos.y, pos.z);
	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;
	//�]��
	PostEffectConstBuffer* constMap = nullptr;
	HRESULT result;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = matWorld * BaseDirectX::GetInstance()->matProjection;
	XMFLOAT4 weight0 = { weights[0], weights[1] ,weights[2] ,weights[3] };
	XMFLOAT4 weight1 = { weights[4], weights[5] ,weights[6] ,weights[7] };
	constMap->weight0 = weight0;
	constMap->weight1 = weight1;
	constMap->frameTime = frameTime;
	frameTime++;
	constBuff->Unmap(0, nullptr);

	//�p�C�v���C���X�e�[�g�ݒ�
	BaseDirectX::GetInstance()->cmdList->SetPipelineState(pipelineState.Get());
	//���[�g�V�O�l�`��
	BaseDirectX::GetInstance()->cmdList->SetGraphicsRootSignature(rootSignature.Get());
	//�v���~�e�B�u�`��
	BaseDirectX::GetInstance()->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ���_�o�b�t�@�̐ݒ�
	BaseDirectX::GetInstance()->cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	// �f�X�N���v�^�q�[�v���Z�b�g
	BaseDirectX::GetInstance()->cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// �萔�o�b�t�@�r���[���Z�b�g
	BaseDirectX::GetInstance()->cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	//�����܂��̎�
	//BaseDirectX::cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	//mulchTex�̎�
	for (int i = 0; i < texNum; i++)
	{
		BaseDirectX::GetInstance()->cmdList->SetGraphicsRootDescriptorTable(i + 1, CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV->GetGPUDescriptorHandleForHeapStart(), i, BaseDirectX::GetInstance()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	}
	// �`��R�}���h
	BaseDirectX::GetInstance()->cmdList->DrawInstanced(4, 1, 0, 0);
}

void PostEffect::PreDraw()
{
	for (int i = 0; i < RenderTarget::renderNum; i++)
	{
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTarget.texBuff[i].Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
		BaseDirectX::GetInstance()->cmdList->ResourceBarrier(1, &barrier);
	}
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH[RenderTarget::renderNum];
	for (int i = 0; i < RenderTarget::renderNum; i++)
	{
		rtvH[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(renderTarget.descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i, BaseDirectX::GetInstance()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	}
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	BaseDirectX::GetInstance()->cmdList->OMSetRenderTargets(RenderTarget::renderNum, rtvH, false, &dsvH);

	CD3DX12_VIEWPORT viewports[RenderTarget::renderNum];
	CD3DX12_RECT scissorRects[RenderTarget::renderNum];
	for (int i = 0; i < RenderTarget::renderNum; i++)
	{
		viewports[i] = CD3DX12_VIEWPORT(0.0f, 0.0f, WindowsAPI::window_width, WindowsAPI::window_height);
		scissorRects[i] = CD3DX12_RECT(0, 0, WindowsAPI::window_width, WindowsAPI::window_height);
	}
	BaseDirectX::GetInstance()->cmdList->RSSetViewports(RenderTarget::renderNum, viewports);
	BaseDirectX::GetInstance()->cmdList->RSSetScissorRects(RenderTarget::renderNum, scissorRects);
	for (int i = 0; i < RenderTarget::renderNum; i++)
	{
		BaseDirectX::GetInstance()->cmdList->ClearRenderTargetView(rtvH[i], renderTarget.clearColor, 0, nullptr);
	}
	BaseDirectX::GetInstance()->cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffect::PostDraw()
{
	for (int i = 0; i < RenderTarget::renderNum; i++)
	{
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTarget.texBuff[i].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		BaseDirectX::GetInstance()->cmdList->ResourceBarrier(1, &barrier);
	}
}

void PostEffect::CreateGraphicsPipelineState(HLSLShader& shader)
{
	HRESULT result = S_FALSE;

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};
	//�O���t�B�b�N�X�p�C�v���C���̐ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(shader.vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(shader.psBlob.Get());
	//�T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//�f�v�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;//��ɏ㏑��
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	for (int i = 0; i < RenderTarget::renderNum; i++)
	{
		gpipeline.BlendState.RenderTarget[i] = blenddesc;
	}
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = texNum;
	for (int i = 0; i < RenderTarget::renderNum; i++)
	{
		gpipeline.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM;
	}
	gpipeline.SampleDesc.Count = 1;

	//�f�X�N���v�^�����W
	//CD3DX12_DESCRIPTOR_RANGE descRangeSRV[PostEffect::texNum];
	for (int i = 0; i < PostEffect::texNum; i++)
	{
		descRangeSRV[i].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, i);//�e�N�X�`��1
	}
	CD3DX12_ROOT_PARAMETER rootparams[PostEffect::texNum + 1]{};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//�萔�o�b�t�@�r���[�Ƃ��ď�����
	for (int i = 1; i <= PostEffect::texNum; i++)
	{
		rootparams[i].InitAsDescriptorTable(1, &descRangeSRV[i - 1], D3D12_SHADER_VISIBILITY_ALL);
	}
	//�X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MINIMUM_MIN_MAG_MIP_POINT);
	//���[�g�V�O�l�`���̐ݒ�
	//CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	//ComPtr<ID3DBlob> rootSigBlob;
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = BaseDirectX::GetInstance()->dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	gpipeline.pRootSignature = rootSignature.Get();
	result = BaseDirectX::GetInstance()->dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));
}

void PostEffect::CalcWeightGaussian(float* weightsTbl, int sizeOfWeightsTbl, float sigma)
{// �d�݂̍��v���L�^����ϐ����`����
	float total = 0;

	// ��������K�E�X�֐���p���ďd�݂��v�Z���Ă���
	// ���[�v�ϐ���x����e�N�Z������̋���
	for (int x = 0; x < sizeOfWeightsTbl; x++)
	{
		weightsTbl[x] = expf(-0.5f * (float)(x * x) / sigma);
		total += 2.0f * weightsTbl[x];
	}

	// �d�݂̍��v�ŏ��Z���邱�ƂŁA�d�݂̍��v��1�ɂ��Ă���
	for (int i = 0; i < sizeOfWeightsTbl; i++)
	{
		weightsTbl[i] /= total;
	}
}

void PostEffects::Init()
{
	postWater.Initialize(ShaderManager::postWater);
	postBlur.Initialize(ShaderManager::postBlur);
	postMosaic.Initialize(ShaderManager::postMosaic);
	postNormal.Initialize(ShaderManager::postNormal);
}

void PostEffects::PreDraw()
{
	type = (PostEffectType)Imgui::GetInstance()->effectType;
	if (type == PostEffectType::Normal)
	{
		postNormal.PreDraw();
	}
	else if (type == PostEffectType::Water)
	{
		postWater.PreDraw();
	}
	else if (type == PostEffectType::Mosaic)
	{
		postMosaic.PreDraw();
	}
	else if (type == PostEffectType::Blur)
	{
		postBlur.PreDraw();
	}
	else
	{
		postNormal.PreDraw();
	}
}

void PostEffects::Draw()
{
	if (Imgui::GetInstance()->effectType < 0) return;
	if (type == PostEffectType::Normal)
	{
		postNormal.Draw();
	}
	else if (type == PostEffectType::Water)
	{
		postWater.Draw();
	}
	else if (type == PostEffectType::Mosaic)
	{
		postMosaic.Draw();
	}
	else if (type == PostEffectType::Blur)
	{

		postBlur.Draw();
	}
	else
	{
		postNormal.Draw();
	}
}

void PostEffects::PostDraw()
{
	if (type == PostEffectType::Normal)
	{
		
		postNormal.PostDraw();
	}
	else if (type == PostEffectType::Water)
	{
		postWater.PostDraw();
	}
	else if (type == PostEffectType::Mosaic)
	{
		postMosaic.PostDraw();
	}
	else if (type == PostEffectType::Blur)
	{
		postBlur.PostDraw();
	}
	else
	{
		postNormal.PostDraw();
	}
}
