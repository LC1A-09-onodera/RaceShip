#include "RenderTarget.h"
#include "../WindowsAPI/WinAPI.h"

const float RenderTarget::clearColor[4] = { 0.5f, 0.5f, 0.5f, 0.0f };

void RenderTarget::TexInit(BaseDirectX &baseDirectX)
{
	//�e�N�X�`���o�b�t�@�̍쐬
	CD3DX12_RESOURCE_DESC texDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, WindowsAPI::window_width, (UINT)WindowsAPI::window_height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
	for (int i = 0; i < RenderTarget::renderNum; i++)
	{
		CD3DX12_HEAP_PROPERTIES heapProp(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
		CD3DX12_CLEAR_VALUE clearValue(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor);
		baseDirectX.result = baseDirectX.dev->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &texDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue, IID_PPV_ARGS(&texBuff[i]));
		assert(SUCCEEDED(baseDirectX.result));

		//�C���[�W�f�[�^�̍쐬
		const UINT pixelCount = WindowsAPI::window_width * WindowsAPI::window_height;
		const UINT rowPitch = sizeof(UINT) * WindowsAPI::window_width;
		const UINT depthPitch = rowPitch * WindowsAPI::window_height;
		UINT* img = new UINT[pixelCount];
		for (int j = 0; j < pixelCount; j++)
		{
			img[j] = 0x000000ff;
		}
		baseDirectX.result = texBuff[i]->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);
		assert(SUCCEEDED(baseDirectX.result));
		delete[] img;
	}
}

void RenderTarget::RTVInit(BaseDirectX& baseDirectX)
{
	//RTV
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = RenderTarget::renderNum;
	baseDirectX.result = baseDirectX.dev->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(baseDirectX.result));
	for (int i = 0; i < RenderTarget::renderNum; i++)
	{
		baseDirectX.dev->CreateRenderTargetView(texBuff[i].Get(), nullptr, CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i, baseDirectX.dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));
	}
}
