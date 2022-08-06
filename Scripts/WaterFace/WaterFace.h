#pragma once
#include "../3DModel/Model.h"
#include "../PostEffect/PostEffect.h"

class Camera;

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
	bool LoadTexture(BaseDirectX& baseDirectX, PostEffect &postEffect);
	void LoadMaterial(BaseDirectX& baseDirectX, const string& directoryPath, const string& filename, PostEffect& postEffect);
	void InitializeGraphicsPipeline(BaseDirectX& baseDirectX, HLSLShader& shader);
	void Update(BaseDirectX& baseDirectX);
	void PreDraw();
	void PostDraw();
	void Draw(BaseDirectX& baseDirectX, WaterEachInfo &f_each);
};

class WaterFace
{
public:
	WaterFaceModel waterModel;
	shared_ptr<Camera> f_camera;
	void LoadModel(BaseDirectX& baseDirectX, HLSLShader &useShader, PostEffect &postEffect);
	void Init(BaseDirectX& baseDirectX);
	void Update();
	void Draw(BaseDirectX& baseDirectX,  XMVECTOR &selingPos);
};

class WaterCameraManager
{
public:
	static list<shared_ptr<Camera>> f_cameras;
};
