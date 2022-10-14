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
	void CreateConstBuff0();
	void CreateConstBuff1();
	void CreateConstBuff2();
	void ConstInit();
};

class WaterFaceModel : public Model
{
public:
	WaterEachInfo eachData;
	ComPtr<ID3D12Resource> depthBuff;
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	PostEffect m_renderTarget;
	UINT frameTime = 0;
	//EachInfo each;
	void CreateModel( const char* name, HLSLShader& shader, bool smoothing = false);
	bool LoadTexture();
	void LoadMaterial( const string& directoryPath, const string& filename);
	void InitializeGraphicsPipeline( HLSLShader& shader);
	void Update();
	void PreDraw();
	void PostDraw();
	void Draw( WaterEachInfo &f_each);
};

class WaterFace
{
public:
	WaterFaceModel waterModel;
	shared_ptr<Camera> m_camera;
	void LoadModel( HLSLShader &useShader, XMFLOAT3 &f_cameraPos, XMFLOAT3 &f_cameraTarget);
	void Init();
	void Update();
	void Draw(  XMVECTOR &selingPos);
};

class WaterCameraManager
{
public:
	static list<shared_ptr<Camera>> f_cameras;
};
