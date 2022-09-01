#include "Model.h"
#include "../Camera/Camera.h"

using namespace std;
shared_ptr<Light> Model::light = nullptr;
std::list<ModelElement*> ModelManager::m_models;
list<EachInfo*> EachManager::eahcs;
Model::~Model()
{

}
inline size_t Model::GetVertexCount()
{
	return mesh.vertices.size();
}

void Model::AddAmoothData(unsigned short indexPosition, unsigned short indexVertex)
{
	smoothData[indexPosition].emplace_back(indexVertex);
}

void Model::CalculateSmoothedVertexNormals()
{
	auto itr = smoothData.begin();
	for (; itr != smoothData.end(); ++itr)
	{
		//���ʒ��_�R���N�V����
		vector<unsigned short>& v = itr->second;
		//�S���_�̖@���̃R���N�V����
		XMVECTOR normal = {};
		for (unsigned short index : v)
		{
			normal += XMVectorSet(mesh.vertices[index].normal.x, mesh.vertices[index].normal.y, mesh.vertices[index].normal.z, 0);
		}
		normal = XMVector3Normalize(normal / (float)v.size());
		//���ʖ@�����g�p����S�Ă̒��_�f�[�^�ɏ�������
		for (unsigned short index : v)
		{
			mesh.vertices[index].normal = { normal.m128_f32[0], normal.m128_f32[1], normal.m128_f32[2] };
		}
	}
}

void Model::SetLight(shared_ptr<Light> Light)
{
	Model::light = Light;
}

void Model::CreateModel(BaseDirectX& baseDirectX, const char* name, HLSLShader& shader, bool smoothing, bool isTriangle)
{
	m_modelName = name;

	InitializeDescriptorHeap(baseDirectX);

	InitializeGraphicsPipeline(baseDirectX, shader, isTriangle);

	LoadFileContents(baseDirectX, name, smoothing);

	ConstBufferInit(baseDirectX, this, this->each);

	ModelElement* model = new ModelElement(m_modelName, this);
	/*if (ModelManager::m_models.size() > 0)
	{
		for (auto itr = ModelManager::m_models.begin(); itr != ModelManager::m_models.end(); ++itr)
		{
			if ((*itr)->m_modelName == m_modelName)
			{
				return;
			}
		}
	}*/
	ModelManager::m_models.push_back(model);
}

void Model::Update(BaseDirectX& baseDirectX, EachInfo* f_each, bool rCamera)
{
	this->each = *f_each;
	CalcMatrix(this, &this->each);
	SendVertex(baseDirectX);
	ConstBufferDataB0* constMap0 = nullptr;
	if (SUCCEEDED(this->each.constBuff0->Map(0, nullptr, (void**)&constMap0)))
	{
		//constMap0->mat = matWorld * Camera::matView * BaseDirectX::matProjection;
		if (!rCamera)
		{
			constMap0->viewproj = Cameras::camera.matView * baseDirectX.matProjection;
			constMap0->world = matWorld;
			constMap0->cameraPos = Cameras::camera.eye;
		}
		else
		{
			constMap0->viewproj = Cameras::rCamera.matView * baseDirectX.matProjection;
			constMap0->world = matWorld;
			constMap0->cameraPos = Cameras::rCamera.eye;
		}

		this->each.constBuff0->Unmap(0, nullptr);
	}
	ConstBufferDataB1* constMap1 = nullptr;
	baseDirectX.result = this->each.constBuff1->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = material.ambient;
	constMap1->diffuse = material.diffuse;
	constMap1->specular = material.specular;
	constMap1->alpha = material.alpha;
	this->each.constBuff1->Unmap(0, nullptr);
}

void Model::Update(BaseDirectX& baseDirectX, EachInfo* f_each, Camera& f_camera)
{
	this->each = *f_each;
	CalcMatrix(this, &this->each);
	SendVertex(baseDirectX);
	ConstBufferDataB0* constMap0 = nullptr;
	if (SUCCEEDED(this->each.constBuff0->Map(0, nullptr, (void**)&constMap0)))
	{
		constMap0->viewproj = f_camera.matView * baseDirectX.matProjection;
		constMap0->world = matWorld;
		constMap0->cameraPos = f_camera.eye;
		this->each.constBuff0->Unmap(0, nullptr);
	}
	ConstBufferDataB1* constMap1 = nullptr;
	baseDirectX.result = this->each.constBuff1->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = material.ambient;
	constMap1->diffuse = material.diffuse;
	constMap1->specular = material.specular;
	constMap1->alpha = material.alpha;
	this->each.constBuff1->Unmap(0, nullptr);
}

void Model::SendVertex(BaseDirectX& baseDirectX)
{
	Vertex* vertMap = nullptr;
	baseDirectX.result = mesh.vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(baseDirectX.result))
	{
		copy(mesh.vertices.begin(), mesh.vertices.end(), vertMap);
		mesh.vertBuff->Unmap(0, nullptr);    // �}�b�v������
	}
}

void Model::LoadFileContents(BaseDirectX& baseDirectX, const char* name, bool smoothing)
{
	ifstream file;
	const string modelname = name;
	const string filename = modelname + ".obj";
	const string directoryPath = "Resource/Model/" + modelname + "/";
	file.open(directoryPath + filename);

	if (file.fail())
	{
		assert(0);
	}

	vector<XMFLOAT3> pos;
	vector<XMFLOAT3> normal;
	vector<XMFLOAT2> uv;

	string line;
	while (getline(file, line))
	{
		istringstream line_stream(line);

		string key;
		getline(line_stream, key, ' ');

		if (key == "mtllib")
		{   //�}�e���A��
			string mFilename;
			line_stream >> mFilename;
			LoadMaterial(baseDirectX, directoryPath, mFilename);
		}
		if (key == "v")
		{   //���W�ǂݍ���
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			pos.emplace_back(position);//�ǉ�
		}
		if (key == "vt")
		{
			XMFLOAT2 texcood{};//UV�󂯎��
			line_stream >> texcood.x;
			line_stream >> texcood.y;

			texcood.y = 1.0f - texcood.y;//���𔽓]
			uv.emplace_back(texcood);

		}
		if (key == "vn")
		{   //�@��
			XMFLOAT3 normals{};
			line_stream >> normals.x;
			line_stream >> normals.y;
			line_stream >> normals.z;
			normal.emplace_back(normals);
		}
		if (key == "f")
		{   //�C���f�b�N�X
			string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				//count += 1;
				istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcood;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexTexcood;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexNormal;

				Vertex verte{};
				verte.pos = pos[indexPosition - 1];
				verte.normal = normal[indexNormal - 1];
				verte.uv = uv[indexTexcood - 1];

				mesh.vertices.emplace_back(verte);
				if (smoothing)
				{
					AddAmoothData(indexPosition, (unsigned short)GetVertexCount() - 1);
				}
				mesh.indices.emplace_back((unsigned short)mesh.indices.size());
			}
		}
	}

	if (smoothing)
	{
		CalculateSmoothedVertexNormals();
	};

	file.close();
}

bool Model::InitializeGraphicsPipeline(BaseDirectX& baseDirectX, HLSLShader& shader, bool isTriangle)
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	if (shader.vsBlob == nullptr)
	{
		assert(0);
	}
	if (shader.psBlob == nullptr)
	{
		assert(0);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(shader.vsBlob.Get());
	if (shader.gsBlob != nullptr)
	{
		gpipeline.GS = CD3DX12_SHADER_BYTECODE(shader.gsBlob.Get());
	}
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(shader.psBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	if (isTriangle)
	{
		gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	}
	else
	{
		gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	}
	// �}�`�̌`��ݒ�i�O�p�`�j
	//gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	// ���[�g�p�����[�^
	//CD3DX12_ROOT_PARAMETER rootparams[3];
	CD3DX12_ROOT_PARAMETER rootparams[4]{};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);
	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ���[�g�V�O�l�`���̐���
	result = baseDirectX.dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	if (FAILED(result)) {
		return false;
	}

	gpipeline.pRootSignature = rootsignature.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���-------------------------
	result = baseDirectX.dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));

	if (FAILED(result)) {
		return false;
	}

	return true;
}

bool Model::LoadTexture(BaseDirectX& baseDirectX, const string& directPath, const string& filename)
{
	HRESULT result = S_FALSE;

	// WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	string filepath = directPath + filename;

	wchar_t wfilepath[128];
	int iBufferSize;
	iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

	result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scratchImg);
	if (FAILED(result))
	{
		return false;
	}

	const Image* img = scratchImg.GetImage(0, 0, 0); // ���f�[�^���o

	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);
	CD3DX12_HEAP_PROPERTIES heapProp(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
	// �e�N�X�`���p�o�b�t�@�̐���
	result = baseDirectX.dev->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // �e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&texbuff));
	if (FAILED(result)) {
		return false;
	}

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = texbuff->WriteToSubresource(
		0,
		nullptr, // �S�̈�փR�s�[
		img->pixels,    // ���f�[�^�A�h���X
		(UINT)img->rowPitch,  // 1���C���T�C�Y
		(UINT)img->slicePitch // 1���T�C�Y
	);
	if (FAILED(result)) {
		return false;
	}

	// �V�F�[�_���\�[�X�r���[�쐬
	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	//BaseDirectX::dev->CreateShaderResourceView(texbuff.Get(), &srvDesc, cpuDescHandleSRV[materialCount]);
	baseDirectX.dev->CreateShaderResourceView(texbuff.Get(), &srvDesc, cpuDescHandleSRV);
	return true;
}

bool Model::InitializeDescriptorHeap(BaseDirectX& baseDirectX)
{
	HRESULT result = S_FALSE;

	// �f�X�N���v�^�q�[�v�𐶐�	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	descHeapDesc.NumDescriptors = 1; // �V�F�[�_�[���\�[�X�r���[1��
	result = baseDirectX.dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//����
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// �f�X�N���v�^�T�C�Y���擾
	descriptorHandleIncrementSize = baseDirectX.dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	return true;
}

void Model::LoadMaterial(BaseDirectX& baseDirectX, const string& directoryPath, const string& filename)
{
	ifstream file;
	file.open(directoryPath + filename);
	if (file.fail())
	{
		assert(0);
	}

	string line;
	while (getline(file, line))
	{
		istringstream line_striam(line);
		string key;
		getline(line_striam, key, ' ');

		if (key[0] == '\t')
		{
			key.erase(key.begin());
		}

		if (key == "newmtl")
		{
			line_striam >> material.name;
		}

		if (key == "Ka")
		{
			line_striam >> material.ambient.x;
			line_striam >> material.ambient.y;
			line_striam >> material.ambient.z;
		}

		if (key == "Kd")
		{
			line_striam >> material.diffuse.x;
			line_striam >> material.diffuse.y;
			line_striam >> material.diffuse.z;
		}

		if (key == "Ks")
		{
			line_striam >> material.specular.x;
			line_striam >> material.specular.y;
			line_striam >> material.specular.z;
		}

		if (key == "map_Kd")
		{
			line_striam >> material.texFilename;
			LoadTexture(baseDirectX, directoryPath, material.texFilename);
		}
	}
	materialCount += 1;
	file.close();
}

void Set3DDraw(BaseDirectX& baseDirectX, const Model& model, bool triangle)
{
	baseDirectX.cmdList->IASetIndexBuffer(&model.mesh.ibView);
	baseDirectX.cmdList->SetPipelineState(model.pipelinestate.Get());
	baseDirectX.cmdList->SetGraphicsRootSignature(model.rootsignature.Get());
	//�O�p�`���X�g
	if (triangle == true)
	{
		baseDirectX.cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//BaseDirectX::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	}

	else if (triangle == false)
	{
		baseDirectX.cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	}
}

void Draw3DObject(BaseDirectX& baseDirectX, const Model& model, bool triangle)
{
	Set3DDraw(baseDirectX, model, triangle);
	baseDirectX.cmdList->IASetVertexBuffers(0, 1, &model.mesh.vbView);
	baseDirectX.cmdList->IASetIndexBuffer(&model.mesh.ibView);
	ID3D12DescriptorHeap* ppHeap[] = { model.descHeap.Get() };
	baseDirectX.cmdList->SetDescriptorHeaps(_countof(ppHeap), ppHeap);
	//BaseDirectX::cmdList->SetGraphicsRootDescriptorTable(0, model.gpuDescHandleCBV);
	baseDirectX.cmdList->SetGraphicsRootConstantBufferView(0, model.each.constBuff0->GetGPUVirtualAddress());
	Model::light->Draw(baseDirectX.cmdList.Get(), 3);
	baseDirectX.cmdList->SetGraphicsRootConstantBufferView(1, model.each.constBuff1->GetGPUVirtualAddress());
	baseDirectX.cmdList->SetGraphicsRootDescriptorTable(2, model.gpuDescHandleSRV);
	baseDirectX.cmdList->DrawIndexedInstanced((UINT)model.mesh.indices.size(), 1, 0, 0, 0);
}

bool ObjectColition(Model& object1, Model& object2)
{
	XMFLOAT3 pos1 = { object1.matWorld.r[3].m128_f32[0], object1.matWorld.r[3].m128_f32[1], object1.matWorld.r[3].m128_f32[2] };
	XMFLOAT3 pos2 = { object2.matWorld.r[3].m128_f32[0], object2.matWorld.r[3].m128_f32[1], object2.matWorld.r[3].m128_f32[2] };
	//���W�̍�
	XMVECTOR position_sub = XMVectorSet(pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z, 0);
	position_sub = XMVector3Length(position_sub);
	float distance = position_sub.m128_f32[0];

	if (distance <= object1.radi + object2.radi)
	{
		return true;
	}

	return false;
}

bool CiycleColition(const XMFLOAT3& object1, const XMFLOAT3& object2, float radi1, float radi2)
{
	//���W�̍�
	XMFLOAT3 position_sub = { object1.x - object2.x, object1.y - object2.y, object1.z - object2.z };
	float distance = sqrtf(position_sub.x * position_sub.x + position_sub.y * position_sub.y + position_sub.z * position_sub.z);;

	if (distance <= radi1 + radi2)
	{
		return true;
	}

	return false;
}
