#pragma once
#include "../3DModel/Model.h"
#include "../PostEffect/PostEffect.h"

class WaterConstBuff0 : public ConstBufferDataB0
{
public:
	UINT frameTime;
};

class WaterEachInfo : public EachInfo
{
public:
	UINT frame = 0;
	void CreateConstBuff0(BaseDirectX& baseDirectX);
	void CreateConstBuff1(BaseDirectX& baseDirectX);
	void CreateConstBuff2(BaseDirectX& baseDirectX);
	void ConstInit(BaseDirectX& baseDirectX);
};

class WaterFaceModel : public Model
{
public:
	WaterEachInfo eachData;
	ComPtr<ID3D12Resource> depthBuff;
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	UINT frameTime = 0;
	//EachInfo each;
	void CreateModel(BaseDirectX& baseDirectX, const char* name, HLSLShader& shader,PostEffect &postEffect, bool smoothing = false);
	bool LoadTexture(BaseDirectX& baseDirectX, const string& directPath, const string& filename, PostEffect &postEffect);
	void LoadMaterial(BaseDirectX& baseDirectX, const string& directoryPath, const string& filename, PostEffect& postEffect);
	void InitializeGraphicsPipeline(BaseDirectX& baseDirectX, HLSLShader& shader, PostEffect &postEffect);
	void Update(BaseDirectX& baseDirectX);
	void PreDraw();
	void PostDraw();
	void Draw(BaseDirectX& baseDirectX, WaterEachInfo&each, PostEffect& postEffect);
};

class WaterFace
{
public:
	WaterFaceModel waterModel;

	void LoadModel(BaseDirectX& baseDirectX, HLSLShader &useShader, PostEffect &postEffect);
	void Init(BaseDirectX& baseDirectX);
	void Update();
	void Draw(BaseDirectX& baseDirectX, PostEffect& postEffect, XMVECTOR &selingPos);
};
