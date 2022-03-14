#include "sampleObject.h"
#include "../Camera/Camera.h"
#include "../Collition/BaseCollision.h"

void SampleObject::Init(int index)
{
    UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * mesh.vertices.size());
    UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * mesh.indices.size());
    &CD3DX12_RESOURCE_DESC::Buffer(sizeVB);
    &CD3DX12_RESOURCE_DESC::Buffer(sizeIB);
    mesh.vbView.SizeInBytes = sizeVB;
    mesh.ibView.SizeInBytes = sizeIB;
    BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(sizeVB), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mesh.vertBuff));
    Vertex* vertMap = nullptr;
    BaseDirectX::result = mesh.vertBuff->Map(0, nullptr, (void**)&vertMap);
    if (SUCCEEDED(BaseDirectX::result))
    {
        copy(mesh.vertices.begin(), mesh.vertices.end(), vertMap);
        mesh.vertBuff->Unmap(0, nullptr);    // マップを解除
    }
    //インデックスバッファの生成
    BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(sizeIB), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mesh.indexBuff));
    unsigned short* indexMap = nullptr;
    BaseDirectX::result = mesh.indexBuff->Map(0, nullptr, (void**)&indexMap);
    if (SUCCEEDED(BaseDirectX::result))
    {
        copy(mesh.indices.begin(), mesh.indices.end(), indexMap);
        mesh.indexBuff->Unmap(0, nullptr);
    }

    // GPU上のバッファに対応した仮想メモリを取得
    mesh.vbView.BufferLocation = mesh.vertBuff->GetGPUVirtualAddress();
    //vbView.SizeInBytes = sizeVB;
    mesh.vbView.StrideInBytes = sizeof(Vertex);
    mesh.ibView.BufferLocation = mesh.indexBuff->GetGPUVirtualAddress();
    mesh.ibView.Format = DXGI_FORMAT_R16_UINT;
    //ibView.SizeInBytes = sizeIB;
    D3D12_HEAP_PROPERTIES heapprop{};
    heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
    //リソース設定
    D3D12_RESOURCE_DESC resdesc{};
    resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resdesc.Width = (sizeof(ConstSampleObject) + 0xff) & ~0xff;
    resdesc.Height = 1;
    resdesc.DepthOrArraySize = 1;
    resdesc.MipLevels = 1;
    resdesc.SampleDesc.Count = 1;
    resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&heapprop, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constBuff0));
    BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constBuff1));

    UINT descHadleIncSize = BaseDirectX::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    cpuDescHandleCBV = BaseDirectX::basicDescHeap->GetCPUDescriptorHandleForHeapStart();
    cpuDescHandleCBV.ptr += index * descHadleIncSize;

    gpuDescHandleCBV = BaseDirectX::basicDescHeap->GetGPUDescriptorHandleForHeapStart();
    gpuDescHandleCBV.ptr += index * descHadleIncSize;

    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
    cbvDesc.BufferLocation = constBuff0->GetGPUVirtualAddress();
    cbvDesc.SizeInBytes = (UINT)constBuff0->GetDesc().Width;
    BaseDirectX::dev->CreateConstantBufferView(&cbvDesc, cpuDescHandleCBV);
    name = typeid(*this).name();
}

void SampleObject::Update()
{
    XMMATRIX matScale, matRot, matTrans;
    const XMFLOAT3& cameraPos = Camera::eye.v;
    matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
    matRot = XMMatrixIdentity();
    matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
    matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
    matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
    matTrans = XMMatrixTranslation(position.m128_f32[0], position.m128_f32[1], position.m128_f32[2]);
    matWorld = XMMatrixIdentity();

    //ビルボード
    //if (billboard)
    //{
    //    matWorld *= BaseDirectX::matBillboard;//ビルボードをかける
    //}
    //ビルボードY
    //if (billboard)
    //{
    //    matWorld *= Camera::matBillboardY;//ビルボードをかける
    //}
    matWorld *= matScale;
    matWorld *= matRot;
    matWorld *= matTrans;

    Vertex* vertMap = nullptr;
    BaseDirectX::result = mesh.vertBuff->Map(0, nullptr, (void**)&vertMap);
    if (SUCCEEDED(BaseDirectX::result))
    {
        copy(mesh.vertices.begin(), mesh.vertices.end(), vertMap);
        mesh.vertBuff->Unmap(0, nullptr);    // マップを解除
    }

    ConstSampleObject* constMap0 = nullptr;
    if (SUCCEEDED(constBuff0->Map(0, nullptr, (void**)&constMap0)))
    {
        //constMap0->mat = matWorld * Camera::matView * BaseDirectX::matProjection;
        constMap0->viewproj = Camera::matView * BaseDirectX::matProjection;
        constMap0->world = matWorld;
        constMap0->cameraPos = cameraPos;
        constMap0->frameTime = frameTime;
        constBuff0->Unmap(0, nullptr);
    }

    ConstBufferDataB1* constMap1 = nullptr;
    BaseDirectX::result = constBuff1->Map(0, nullptr, (void**)&constMap1);
    constMap1->ambient = material.ambient;
    constMap1->diffuse = material.diffuse;
    constMap1->specular = material.specular;
    constMap1->alpha = material.alpha;
    constBuff1->Unmap(0, nullptr);
    if (collider)
    {
        collider->Update();
    }
}
