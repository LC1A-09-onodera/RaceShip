#include "SpriteCommon.h"

//�O���t�B�b�N�X�p�C�v���C��
D3D12_GRAPHICS_PIPELINE_STATE_DESC SpriteCommon::gpipeline;
//�X�v���C�g�p�f�X�N���v�^�q�[�v
ComPtr<ID3D12DescriptorHeap> SpriteCommon::basicDescHeap;
D3D12_INPUT_ELEMENT_DESC SpriteCommon::inputLayout[2];
ComPtr<ID3D12PipelineState> SpriteCommon::spritePipelineState;
XMMATRIX SpriteCommon::matProjection{};
ComPtr<ID3D12DescriptorHeap> SpriteCommon::DescHeap;
ComPtr<ID3D12Resource> SpriteCommon::TexBuff[SRVCount];


SpriteCommon::SpriteCommon()
{
	
}

void SpriteCommon::Init(BaseDirectX &baseDirectX, ComPtr<ID3DBlob> vsBlob, ComPtr<ID3DBlob> psBlob)
{

    SpriteCommon::inputLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
    SpriteCommon::inputLayout[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
    gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
    gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
    gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
    gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�w�ʃJ�����O�����Ȃ�
    gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
    gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    D3D12_RENDER_TARGET_BLEND_DESC &blenddesc = gpipeline.BlendState.RenderTarget[0];
    blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    //���ʐݒ�------------------------------------------
    blenddesc.BlendEnable = true;
    blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
    blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
    blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
    //���Z����------------------------------------------
    /*blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
    blenddesc.SrcBlend = D3D12_BLEND_ONE;
    blenddesc.DestBlend = D3D12_BLEND_ONE;*/
    //���Z����------------------------------------------
    /*blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
    blenddesc.SrcBlend = D3D12_BLEND_ONE;
    blenddesc.DestBlend = D3D12_BLEND_ONE;*/
    //�F�̔��]------------------------------------------
    /*blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
    blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
    blenddesc.DestBlend = D3D12_BLEND_ZERO;*/
    //����������
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
    blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
    blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    //���_���C�A�E�g�̐ݒ�
    gpipeline.InputLayout.pInputElementDescs = inputLayout;
    gpipeline.InputLayout.NumElements = _countof(inputLayout);
    //�}�`���O�p�`��
    gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    gpipeline.NumRenderTargets = 1; // �`��Ώۂ�1��
    gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
    gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O
    /*descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    descHeapDesc.NumDescriptors = ConstBufferNum + 1;*/
    //BaseDirectX::result = BaseDirectX::dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));
    //�ˉe�s��
    matProjection = XMMatrixOrthographicOffCenterLH(0.0f, static_cast<float>(window_width), static_cast<float>(window_height), 0.0f, 0.0f, 1.0f);
    D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
    descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    descHeapDesc.NumDescriptors = SRVCount;
    baseDirectX.result = baseDirectX.dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&DescHeap));

}

HRESULT SpriteCommon::LoadSprtieGraph(const wchar_t *filename)
{
    //�摜�f�[�^
    TexMetadata metadata{};
    ScratchImage scratchImg{};
    HRESULT result;
    result = LoadFromWICFile(filename, WIC_FLAGS_NONE, &metadata, scratchImg);
    return S_OK;
}
