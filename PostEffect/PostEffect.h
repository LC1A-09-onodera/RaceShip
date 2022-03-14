#pragma once
#include "../Sprite/Sprite.h"
#include "../RenderTarget/RenderTarget.h"

class PostEffect : public Sprite
{
public:
	PostEffect();
	void Initialize();
	void Draw();
	void PreDraw();
	void PostDraw();
	void CreateGraphicsPipelineState();

	void CalcWeightGaussian(float* weightsTbl, int sizeOfWeightsTbl, float sigma);
	
	RenderTarget renderTarget;
	static const int texNum = 2;
	static UINT frameTime;
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	//震度バッファ
	ComPtr<ID3D12Resource> depthBuff;
	//DST用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	ComPtr<ID3D12PipelineState> pipelineState;
	ComPtr<ID3D12RootSignature> rootSignature;

	static const int NumWeight = 8;
	float weights[NumWeight];
};