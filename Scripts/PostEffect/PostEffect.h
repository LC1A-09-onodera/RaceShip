#pragma once
#include "../Sprite/Sprite.h"
#include "../RenderTarget/RenderTarget.h"
#include "../Shader/ShaderManager.h"

class PostEffectConstBuffer : public ConstBufferData
{
	public:
	float postEffectType;
};

class PostEffect : public Sprite
{
	XMFLOAT3 pos = {0, 0, 0};
	XMFLOAT3 scale = {1.0f, 1.0f, 1.0f};
	XMFLOAT3 rotation = { 0, 0, 0 };
	
	static const int texNum = 1;
	static UINT frameTime;
	static float effectType;
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	//�f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV[1] = {};
	//�k�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuff;
	//DST�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	ComPtr<ID3D12PipelineState> pipelineState;
	ComPtr<ID3D12RootSignature> rootSignature;

	static const int NumWeight = 8;
	float weights[NumWeight] = {};
public:
	PostEffect();
	void Initialize(HLSLShader &shader);
	void Draw();
	void PreDraw();
	void PostDraw();
	void CreateGraphicsPipelineState(HLSLShader &shader);
	void CalcWeightGaussian(float* weightsTbl, int sizeOfWeightsTbl, float sigma);
	RenderTarget renderTarget;
};



class PostEffects
{
public:
	enum class PostEffectType
	{
		Normal = 0,
		Water = 1,
		Mosaic = 2,
		Blur = 3,
	};
	static PostEffect postWater;
	static PostEffect postMosaic;
	static PostEffect postBlur;
	static PostEffect postNormal;
	static PostEffectType type;
	static void Init();
	static void PreDraw();
	static void Draw();
	static void PostDraw();
};