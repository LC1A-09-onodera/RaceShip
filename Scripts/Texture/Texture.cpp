#include "Texture.h"

int TexManager::texNum = 0;

//��m�̎擾
void Tex::LoadGraph(BaseDirectX &baseDirectX, const wchar_t *graph)
{
    this->texNum = TexManager::texNum;
    TexManager::texNum += 1;
    TexMetadata metadata{};
    ScratchImage scratchImg{};
    baseDirectX.result = LoadFromWICFile(graph, WIC_FLAGS_NONE, &metadata, scratchImg);
    const Image *img = scratchImg.GetImage(0, 0, 0);//���f�[�^�̒��o

    CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(metadata.format, metadata.width, (UINT)metadata.height, (UINT16)metadata.arraySize, (UINT16)metadata.mipLevels);
    CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
    baseDirectX.result = baseDirectX.dev->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &texresDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&texbuff));
    baseDirectX.result = texbuff->WriteToSubresource(0, nullptr, img->pixels, (UINT)img->rowPitch, (UINT)img->slicePitch);

    //�摜�f�[�^�g�p��
    //�V�F�[�_�[���\�[�X�r���[
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = metadata.format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    baseDirectX.dev->CreateShaderResourceView(texbuff.Get(), &srvDesc, baseDirectX.cpuDescHandleSRV[this->texNum]);
}

int Tex::Get()
{
    return this->texNum;
}

