#include "Sprite.h"


SpriteCommon Sprite::common;

void Sprite::CreateSprite(BaseDirectX& baseDirectX, const wchar_t* graph, XMFLOAT3 f_position, ComPtr<ID3D12Resource> texBuff, bool f_back, bool TexSize)
{
    tex.LoadGraph(baseDirectX, graph);
    texNum = tex.Get();
    this->back = f_back;
    VertexPosUv Spritevertices[] = {
        {{  0.0f, 100.0f, 0.0f}, {0.0f, 1.0f}},
        {{  0.0f,   0.0f, 0.0f}, {0.0f, 0.0f}},
        {{100.0f, 100.0f, 0.0f}, {1.0f, 1.0f}},
        {{100.0f,   0.0f, 0.0f}, {1.0f, 0.0f}},
    };
    //���_�V�F�[�_�[�̓ǂݍ��݃s�N�Z���V�F�[�_�[�̓ǂݍ��ݒ��_���C�A�E�g�̓ǂݍ���
    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    baseDirectX.result = D3DCompileFromFile(L"Resource/HLSL/spriteVertexShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vsBlob, &errorBlob);

    if (FAILED(baseDirectX.result)) {
        // errorBlob����G���[���e��string�^�ɃR�s�[
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char *)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
        errstr += "\n";

        // �G���[���e���o�̓E�B���h�E�ɕ\��
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }

    // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    baseDirectX.result = D3DCompileFromFile( L"Resource/HLSL/spritePixelShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &psBlob, &errorBlob);

    if (FAILED(baseDirectX.result)) {
        // errorBlob����G���[���e��string�^�ɃR�s�[
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char *)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        // �G���[���e���o�̓E�B���h�E�ɕ\��
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }

    int sizevb = sizeof(Spritevertices);

    //���ʃf�[�^�̏�����
    common.Init(baseDirectX, vsBlob, psBlob);

    this->position.m128_f32[0] = f_position.x;
    this->position.m128_f32[1] = f_position.y;
    this->position.m128_f32[2] = f_position.z;

    //�f�X�N���v�^�����W
    descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
    rootParams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//�萔�o�b�t�@�r���[�Ƃ��ď�����
    rootParams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
    rootSignatureDesc.Init_1_0(_countof(rootParams), rootParams, 1, &baseDirectX.samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
    baseDirectX.result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
    baseDirectX.result = baseDirectX.dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&spriteRootSignature));
    common.gpipeline.pRootSignature = spriteRootSignature.Get();

    baseDirectX.result = baseDirectX.dev->CreateGraphicsPipelineState(&common.gpipeline, IID_PPV_ARGS(&common.spritePipelineState));

    D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
    descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    descHeapDesc.NumDescriptors = common.SRVCount;
    baseDirectX.result = baseDirectX.dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&common.DescHeap));
    
    enum{LB,LT,RB,RT};

    if (TexSize == true)
    {
        D3D12_RESOURCE_DESC resDesc;
        if (texBuff == nullptr)
        {
            resDesc = tex.texbuff->GetDesc();
        }
        else
        {
            resDesc = texBuff->GetDesc();
        }

        float wid = (float)resDesc.Width;
        float hei = (float)resDesc.Height;

        Spritevertices[LB].pos = {0.0f, hei, 0.0f};
        Spritevertices[LT].pos = {0.0f, 0.0f,0.0f};
        Spritevertices[RB].pos = {wid, hei, 0.0f};
        Spritevertices[RT].pos = {wid, 0.0f,0.0f};
    }

    //���_�}�b�v
    CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizevb);
    baseDirectX.result = baseDirectX.dev->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff));
    VertexPosUv *vertMap = nullptr;
    baseDirectX.result = vertBuff->Map(0, nullptr, (void **)&vertMap);
    memcpy(vertMap, Spritevertices, sizeof(Spritevertices));
    vertBuff->Unmap(0, nullptr);
    vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
    vbView.SizeInBytes = sizeof(Spritevertices);
    vbView.StrideInBytes = sizeof(Spritevertices[0]);
    CD3DX12_RESOURCE_DESC resourceDescConst = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);
    baseDirectX.result = baseDirectX.dev->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDescConst, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constBuff));
    
    //�萔�}�b�v
    ConstBufferDataSP *constMap = nullptr;
    baseDirectX.result = constBuff->Map(0, nullptr, (void **)&constMap);
    constMap->color = XMFLOAT4(1, 1, 1, 1);
    constMap->mat = XMMatrixOrthographicOffCenterLH(0.0f, static_cast<float>(window_width), static_cast<float>(window_height), 0.0f, 0.0f, 1.0f);
    constBuff->Unmap(0, nullptr);
}

void Sprite::CreateSprite(BaseDirectX& baseDirectX, Tex f_tex, XMFLOAT3 f_position, ComPtr<ID3D12Resource> texBuff, bool f_back, bool TexSize)
{
    this->tex = f_tex;
    texNum = this->tex.Get();
    this->back = f_back;
    VertexPosUv Spritevertices[] = {
        {{  0.0f, 100.0f, 0.0f}, {0.0f, 1.0f}},
        {{  0.0f,   0.0f, 0.0f}, {0.0f, 0.0f}},
        {{100.0f, 100.0f, 0.0f}, {1.0f, 1.0f}},
        {{100.0f,   0.0f, 0.0f}, {1.0f, 0.0f}},
    };
    //���_�V�F�[�_�[�̓ǂݍ��݃s�N�Z���V�F�[�_�[�̓ǂݍ��ݒ��_���C�A�E�g�̓ǂݍ���
    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    baseDirectX.result = D3DCompileFromFile(L"Resource/HLSL/spriteVertexShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vsBlob, &errorBlob);

    if (FAILED(baseDirectX.result)) {
        // errorBlob����G���[���e��string�^�ɃR�s�[
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
        errstr += "\n";

        // �G���[���e���o�̓E�B���h�E�ɕ\��
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }

    // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    baseDirectX.result = D3DCompileFromFile(L"Resource/HLSL/spritePixelShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &psBlob, &errorBlob);

    if (FAILED(baseDirectX.result)) {
        // errorBlob����G���[���e��string�^�ɃR�s�[
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        // �G���[���e���o�̓E�B���h�E�ɕ\��
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }

    int sizevb = sizeof(Spritevertices);

    //���ʃf�[�^�̏�����
    common.Init(baseDirectX, vsBlob, psBlob);

    this->position.m128_f32[0] = f_position.x;
    this->position.m128_f32[1] = f_position.y;
    this->position.m128_f32[2] = f_position.z;

    //�f�X�N���v�^�����W
    descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
    rootParams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//�萔�o�b�t�@�r���[�Ƃ��ď�����
    rootParams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
    rootSignatureDesc.Init_1_0(_countof(rootParams), rootParams, 1, &baseDirectX.samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
    baseDirectX.result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
    baseDirectX.result = baseDirectX.dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&spriteRootSignature));
    common.gpipeline.pRootSignature = spriteRootSignature.Get();

    baseDirectX.result = baseDirectX.dev->CreateGraphicsPipelineState(&common.gpipeline, IID_PPV_ARGS(&common.spritePipelineState));

    D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
    descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    descHeapDesc.NumDescriptors = common.SRVCount;
    baseDirectX.result = baseDirectX.dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&common.DescHeap));

    enum { LB, LT, RB, RT };

    if (TexSize == true)
    {
        D3D12_RESOURCE_DESC resDesc;
        if (texBuff == nullptr)
        {
            resDesc = this->tex.texbuff->GetDesc();
        }
        else
        {
            resDesc = texBuff->GetDesc();
        }

        float wid = (float)resDesc.Width;
        float hei = (float)resDesc.Height;

        Spritevertices[LB].pos = { 0.0f, hei, 0.0f };
        Spritevertices[LT].pos = { 0.0f, 0.0f,0.0f };
        Spritevertices[RB].pos = { wid, hei, 0.0f };
        Spritevertices[RT].pos = { wid, 0.0f,0.0f };
    }

    //���_�}�b�v
    CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizevb);
    baseDirectX.result = baseDirectX.dev->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff));
    VertexPosUv* vertMap = nullptr;
    baseDirectX.result = vertBuff->Map(0, nullptr, (void**)&vertMap);
    memcpy(vertMap, Spritevertices, sizeof(Spritevertices));
    vertBuff->Unmap(0, nullptr);
    vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
    vbView.SizeInBytes = sizeof(Spritevertices);
    vbView.StrideInBytes = sizeof(Spritevertices[0]);
    CD3DX12_RESOURCE_DESC resourceDescConst = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);
    baseDirectX.result = baseDirectX.dev->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDescConst, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constBuff));

    //�萔�}�b�v
    ConstBufferDataSP* constMap = nullptr;
    baseDirectX.result = constBuff->Map(0, nullptr, (void**)&constMap);
    constMap->color = XMFLOAT4(1, 1, 1, 1);
    constMap->mat = XMMatrixOrthographicOffCenterLH(0.0f, static_cast<float>(window_width), static_cast<float>(window_height), 0.0f, 0.0f, 1.0f);
    constBuff->Unmap(0, nullptr);
}

void Sprite::SpritePipelineEdit(BaseDirectX& baseDirectX)
{
    //�p�C�v���C���X�e�[�g�ݒ�
    baseDirectX.cmdList->SetPipelineState(common.spritePipelineState.Get());
    //���[�g�V�O�l�`��
    baseDirectX.cmdList->SetGraphicsRootSignature(spriteRootSignature.Get());
    //�v���~�e�B�u�`��
    baseDirectX.cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Sprite::SpriteDraw(BaseDirectX& baseDirectX)
{
    Move(baseDirectX);
    SpritePipelineEdit(baseDirectX);
    //�f�X�N���v�^�q�[�v�̔z��
    ID3D12DescriptorHeap *ppHeap[] = { baseDirectX.basicDescHeap.Get() };
    baseDirectX.cmdList->SetDescriptorHeaps(_countof(ppHeap), ppHeap);
    //���_�o�b�t�@�̃Z�b�g
    baseDirectX.cmdList->IASetVertexBuffers(0, 1, &vbView);
    //�萔�o�b�t�@
    baseDirectX.cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
    //�V�F�[�_�[���\�[�X�r���[
    baseDirectX.cmdList->SetGraphicsRootDescriptorTable(1, baseDirectX.gpuDescHandleSRV[texNum]);
    //�`��R�}���h
    baseDirectX.cmdList->DrawInstanced(4,1,0,0);

    if (back == true)
    {
        baseDirectX.BackGroundFront();
    }
}

void Sprite::Move(BaseDirectX& baseDirectX)
{
    //���[���h�s��X�V
    matWorld = XMMatrixIdentity();
    matWorld *= XMMatrixRotationZ(rotation);
    matWorld *= XMMatrixTranslationFromVector(position);
    //�]��
    ConstBufferDataSP *constMap = nullptr;
    baseDirectX.result = constBuff->Map(0, nullptr, (void **)&constMap);
    constMap->mat = matWorld * common.matProjection;
    constBuff->Unmap(0, nullptr);
}

void Sprite::ChangeSize(BaseDirectX& baseDirectX, float wid, float hei)
{
    VertexPosUv Spritevertices[] = {
        {{  0.0f, 100.0f, 0.0f}, {0.0f, 1.0f}},
        {{  0.0f,   0.0f, 0.0f}, {0.0f, 0.0f}},
        {{100.0f, 100.0f, 0.0f}, {1.0f, 1.0f}},
        {{100.0f,   0.0f, 0.0f}, {1.0f, 0.0f}},
    };

    int sizevb = sizeof(Spritevertices);

    enum{LB,LT,RB,RT};

    Spritevertices[LB].pos = {0.0f, hei, 0.0f};
    Spritevertices[LT].pos = {0.0f, 0.0f,0.0f};
    Spritevertices[RB].pos = {wid, hei, 0.0f};
    Spritevertices[RT].pos = {wid, 0.0f,0.0f};
    

    //���_�}�b�v
    CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizevb);
    baseDirectX.result = baseDirectX.dev->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff));
    VertexPosUv *vertMap = nullptr;
    baseDirectX.result = vertBuff->Map(0, nullptr, (void **)&vertMap);
    memcpy(vertMap, Spritevertices, sizeof(Spritevertices));
    vertBuff->Unmap(0, nullptr);
    vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
    vbView.SizeInBytes = sizeof(Spritevertices);
    vbView.StrideInBytes = sizeof(Spritevertices[0]);
    CD3DX12_RESOURCE_DESC resourceDescConst = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);
    baseDirectX.result = baseDirectX.dev->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDescConst, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constBuff));
    
    //�萔�}�b�v
    ConstBufferDataSP *constMap = nullptr;
    baseDirectX.result = constBuff->Map(0, nullptr, (void **)&constMap);
    constMap->color = XMFLOAT4(1, 1, 1, 1);
    constMap->mat = XMMatrixOrthographicOffCenterLH(0.0f, static_cast<float>(window_width), static_cast<float>(window_height), 0.0f, 0.0f, 1.0f);
    constBuff->Unmap(0, nullptr);
}

void Sprite::ChangeSizeOther(BaseDirectX& baseDirectX, float wid, float hei)
{
    VertexPosUv Spritevertices[] = {
           {{  0.0f, 100.0f, 0.0f}, {0.0f, 1.0f}},
           {{  0.0f,   0.0f, 0.0f}, {0.0f, 0.0f}},
           {{100.0f, 100.0f, 0.0f}, {1.0f, 1.0f}},
           {{100.0f,   0.0f, 0.0f}, {1.0f, 0.0f}},
    };

    int sizevb = sizeof(Spritevertices);

    enum { LB, LT, RB, RT };

    Spritevertices[LB].pos = { -(wid / 2), hei / 2, 0.0f };
    Spritevertices[LT].pos = { -(wid / 2), -(hei / 2),0.0f };
    Spritevertices[RB].pos = { wid / 2, hei / 2, 0.0f };
    Spritevertices[RT].pos = { wid / 2, -(hei / 2),0.0f };


    //���_�}�b�v
    CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizevb);
    baseDirectX.result = baseDirectX.dev->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff));
    VertexPosUv* vertMap = nullptr;
    baseDirectX.result = vertBuff->Map(0, nullptr, (void**)&vertMap);
    memcpy(vertMap, Spritevertices, sizeof(Spritevertices));
    vertBuff->Unmap(0, nullptr);
    vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
    vbView.SizeInBytes = sizeof(Spritevertices);
    vbView.StrideInBytes = sizeof(Spritevertices[0]);
    CD3DX12_RESOURCE_DESC resourceDescConst = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);
    baseDirectX.result = baseDirectX.dev->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resourceDescConst, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constBuff));

    //�萔�}�b�v
    ConstBufferDataSP* constMap = nullptr;
    baseDirectX.result = constBuff->Map(0, nullptr, (void**)&constMap);
    constMap->color = XMFLOAT4(1, 1, 1, 1);
    constMap->mat = XMMatrixOrthographicOffCenterLH(0.0f, static_cast<float>(window_width), static_cast<float>(window_height), 0.0f, 0.0f, 1.0f);
    constBuff->Unmap(0, nullptr);
}
