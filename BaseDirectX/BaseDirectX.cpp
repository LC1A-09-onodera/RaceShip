#include "BaseDirectX.h"
#include "viewport.h"
#include "Input.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx12.h"
#include "../imgui/imgui_impl_win32.h"

BaseDirectX::BaseDirectX()
{
	
}

BaseDirectX::~BaseDirectX()
{
	
}

void BaseDirectX::DebugLayerOn()
{

#ifdef DEBUG
	comPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif // DEBUG
#ifdef _DEBUG
	//デバッグレイヤーをオンに
	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif
}

void BaseDirectX::Set()
{
	DebugLayerOn();
	WindowsAPI::Set();
	// DXGIファクトリーの生成
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));

	for (int i = 0; dxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter); // 動的配列に追加する
	}

	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC1 adesc;
		adapters[i]->GetDesc1(&adesc);
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}

		std::wstring strDesc = adesc.Description;
		if (strDesc.find(L"Intel") == std::wstring::npos)
		{
			tmpAdapter = adapters[i];
			break;
		}
	}

	levels[0] = D3D_FEATURE_LEVEL_12_1;
	levels[1] = D3D_FEATURE_LEVEL_12_0;
	levels[2] = D3D_FEATURE_LEVEL_11_1;
	levels[3] = D3D_FEATURE_LEVEL_11_0;

	for (int i = 0; i < _countof(levels); i++)
	{
		// 採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter, levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK)
		{
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}

	// コマンドアロケータを生成
	result = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator));
	// コマンドリストを生成
	result = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&cmdList));
	// 標準設定でコマンドキューを生成
	dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));

	swapchainDesc.Width = 1280;
	swapchainDesc.Height = 720;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapchainDesc.BufferCount = 2;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	result = dxgiFactory->CreateSwapChainForHwnd(cmdQueue.Get(), WindowsAPI::hwnd, &swapchainDesc, nullptr, nullptr, &swapchain1);
	swapchain1.As(&swapchain);

	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー
	heapDesc.NumDescriptors = 2;    // 裏表の２つ
	dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));

	backBuffers.resize(2);
	for (int i = 0; i < 2; i++)
	{
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		//新コード
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(), i, dev->GetDescriptorHandleIncrementSize(heapDesc.Type));
		dev->CreateRenderTargetView(backBuffers[i].Get(), nullptr, handle);
	}
	result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, window_width, window_height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
	result = dev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &depthResDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0), IID_PPV_ARGS(&depthBuffer));
	//result = dev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(vert))
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	result = dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(depthBuffer.Get(), &dsvDesc, dsvHeap->GetCPUDescriptorHandleForHeapStart());

	matProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), (float)window_width / window_height, 0.1f, 1000.0f);

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(L"Resource/HLSL/VertexShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vsBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
		errstr += "\n";

		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(L"Resource/HLSL/PixelShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &psBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	inputLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	inputLayout[1] = { "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	inputLayout[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	descRangeCRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	rootparam[0].InitAsDescriptorTable(1, &descRangeCRV);
	rootparam[1].InitAsDescriptorTable(1, &descRangeSRV);
	depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, window_width, window_height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
	result = dev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &depthResDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0), IID_PPV_ARGS(&depthBuffer));
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	result = dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(depthBuffer.Get(), &dsvDesc, dsvHeap->GetCPUDescriptorHandleForHeapStart());
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//深度テスト
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//ブレンドステート
	//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;  // RBGA全てのチャンネルを描画
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//共通設定------------------------------------------
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	//加算合成------------------------------------------
	/*blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;*/
	//減算合成------------------------------------------
	/*blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;*/
	//色の反転------------------------------------------
	/*blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
	blenddesc.DestBlend = D3D12_BLEND_ZERO;*/
	//半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	//頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);
	//図形を三角形に
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	gpipeline.NumRenderTargets = 1; // 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	//ルートシグネチャ-------------------------------------------
	//ComPtr<ID3D12RootSignature> rootsignature;
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Init_1_0(_countof(rootparam), rootparam, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	/*if (FAILED(result))
	{
		return result;
	}*/
	//------------------------------------------------------------
	// パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature.Get();
	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));
	//定数バッファデスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = TexBufferNum;
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));
	D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle = basicDescHeap->GetCPUDescriptorHandleForHeapStart();
}

void BaseDirectX::UpdateFront()
{
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += static_cast<SIZE_T>(bbIndex) * static_cast<SIZE_T>(dev->GetDescriptorHandleIncrementSize(heapDesc.Type));
	dsvH = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	//背景
	/*clearColor[0] = 0.2f;
	clearColor[1] = 0.4f;
	clearColor[2] = 0.7f;
	clearColor[3] = 0.0f;*/
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	//ビューポート設定
	Viewport::ViewPortUpdate();
	cmdList->RSSetViewports(1, &viewport);
	//シザー矩形設定
	Scissorrect::ScissorrectUpdate();
	cmdList->RSSetScissorRects(1, &scissorrect);
}

void BaseDirectX::BackGroundFront()
{
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void BaseDirectX::UpdateBack()
{
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
	cmdList->Close();

	//コマンドリストの実行
	ID3D12CommandList* cmdLists[] = { cmdList.Get() };
	cmdQueue->ExecuteCommandLists(1, cmdLists);
	//バッファのフリップ
	swapchain->Present(WindowsAPI::intarval, 0);
	//swapchain->Present(1, 0);
	//コマンドキューの実行完了を待つ
	cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		if (event != 0)
		{
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
	}
	cmdAllocator->Reset();
	cmdList->Reset(cmdAllocator.Get(), nullptr);

}

void BaseDirectX::GetAdress()
{
	for (int i = 0; i < TexBufferNum; i++)
	{
		cpuDescHandleSRV[i] = basicDescHeap->GetCPUDescriptorHandleForHeapStart();
		gpuDescHandleSRV[i] = basicDescHeap->GetGPUDescriptorHandleForHeapStart();
		cpuDescHandleSRV[i].ptr += static_cast<UINT64>(i) * static_cast<UINT64>(dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
		gpuDescHandleSRV[i].ptr += static_cast<UINT64>(i) * static_cast<UINT64>(dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	}
}