#pragma once
#include <DirectXMath.h>
#include <d3dcompiler.h>

#pragma warning(push)
#pragma warning(disable:6001)
#include <d3dx12.h>
#pragma warning(pop)
#include "../Light/PointLight.h"
#include "../Light/SpotLight.h"
#include "../Light/CircleShadow.h"
#pragma warning(push)
#pragma warning(disable:4505)
#include "../BaseDirectX/DX12operator.h"
#pragma warning(pop)

using namespace Microsoft::WRL;
using namespace DirectX;

class Light
{
private:
	static ID3D12Device *device;
	static const int PointLightNum = 3;
	static const int SpotLightNum = 3;
	static const int CircleShadowNum = 1;
	
public:
	
	struct ConstBufferData
	{
		XMVECTOR lightv;	//���C�g�̕����x�N�g��
		XMFLOAT3 lightcolor;//���C�g�̐F
		float pad1;
		PointLight::ConstBuffData pointLights[PointLightNum];
		//float pad2;
		SpotLight::ConstBufferData spotLights[SpotLightNum];
		CircleShadow::ConstBufferData circleShadows[CircleShadowNum];
	};
	static void StaticInitialize(ID3D12Device *Device);
	void Update();
	void Draw(ID3D12GraphicsCommandList *cmdList, UINT rootParameterIndex);
	static Light *Create();
	void SetLightDir(const XMVECTOR &lightDir);
	void SetLightColor(const XMFLOAT3 &lightColor);
	void SetPointLightActive(int index, bool active);
	void SetPointLightPos(int index, const XMFLOAT3 &lightpos);
	void SetPointLightColor(int index, const XMFLOAT3 &lightColor);
	void SetPointLightAtten(int index, const XMFLOAT3 &lightAtten);

	void SetSpotLightActive(int index, bool active);
	void SetSpotLightDir(int index, const XMVECTOR &lightDir);
	void SetSpotLightPos(int index, const XMFLOAT3 &lightPos);
	void SetSpotLightColor(int index, const XMFLOAT3 &lightColor);
	void SetSpotLightAtten(int index, const XMFLOAT3 &lightAtten);
	void SetSpotLightAngle(int index, const XMFLOAT2 &lightFactorAngle);

	void SetCircleShadowActive(int index, bool active);
	void SetCircleShadowCasterPos(int index, const XMFLOAT3 &casterPos);
	void SetCircleShadowDir(int index, XMVECTOR &lightdir);
	void SetCircleShadowDistanceCasterLight(int index, float distanceCasterLight);
	void SetCircleShadowAtten(int index, const XMFLOAT3 lightAtten);
	void SetCircleShadowFactorAngle(int index, const XMFLOAT2 &lightfactorAngle);
	void SetLightDir(XMFLOAT3 dir){lightdir = ConvertXMFLOAT3toXMVECTOR(dir);}
	void SetDirty(bool dirtyFlag) { dirty = dirtyFlag; }

private:
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	float pad1 = 0.0f;
	float pad2 = 0.0f;
	//���C�g�����x�N�g��
	XMVECTOR lightdir = {0, 0, 1.0f, 0};
	//���C�g�̐F
	XMFLOAT3 lightcolor = {0.5f, 0.5f, 0.5f};
	//�_�[�e�B�t���O
	bool dirty = false;
	void CreateBuff();
	void Init();
	void TransConstBuffer();
	
	PointLight pointLight[PointLightNum];
	SpotLight spotLights[SpotLightNum];
	CircleShadow circleShadows[CircleShadowNum];
};