#include "Light.h"

ID3D12Device *Light::device = nullptr;

void Light::StaticInitialize(ID3D12Device* Device)
{
	//�ď������`�F�b�N
	assert(!Light::device);
	assert(Device);
	Light::device = (Device);
}

void Light::CreateBuff()
{
	HRESULT result;
	CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) &~0xff);
	result = device->CreateCommittedResource(&heapProp,
			 D3D12_HEAP_FLAG_NONE,
			 &resourceDesc,
			 D3D12_RESOURCE_STATE_GENERIC_READ,
			 nullptr,
			 IID_PPV_ARGS(&constBuff));
	if (FAILED(result))
	{
		assert(0);
	}
	TransConstBuffer();
}

void Light::TransConstBuffer()
{
	HRESULT result;
	ConstBufferData *constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->lightv = -lightdir;
		constMap->lightcolor = lightcolor;
		for (int i = 0; i < PointLightNum; i++)
		{
			if (pointLight[i].isActive())
			{
				constMap->pointLights[i].active = 1;
				constMap->pointLights[i].lightpos = pointLight[i].GetLightPos();
				constMap->pointLights[i].lightcolor = pointLight[i].GetColor();
				constMap->pointLights[i].lighttatten = pointLight[i].GetAtten();
			}
			else
			{
				constMap->pointLights[i].active = 0;
			}
		}
		for (int i = 0; i < SpotLightNum; i++)
		{
			if (spotLights[i].isActive())
			{
				constMap->spotLights[i].active = 1;
				constMap->spotLights[i].lightv = -spotLights[i].GetLightDir();
				constMap->spotLights[i].lightpos = spotLights[i].GetLightPos();
				constMap->spotLights[i].lightcolor = spotLights[i].GetLightColor();
				constMap->spotLights[i].lightatten = spotLights[i].GetLightAtten();
				constMap->spotLights[i].lightfactoranglecos = spotLights[i].GetLightFactorAngleCos();
			}
			else
			{
				constMap->spotLights[i].active = 0;
			}
		}
		for (int i = 0; i < CircleShadowNum; i++)
		{
			if (circleShadows[i].isActive())
			{
				constMap->circleShadows[i].active = 1;
				constMap->circleShadows[i].dir = -circleShadows[i].GetDir();
				constMap->circleShadows[i].casterPos = circleShadows[i].GetCasterPos();
				constMap->circleShadows[i].distanceCasterLight = circleShadows[i].GetDistanceCasterLight();
				constMap->circleShadows[i].atten = circleShadows[i].GetAtten();
				constMap->circleShadows[i].factorAngleCos = circleShadows[i].GetFactorAngleCos();
			}
			else
			{
				constMap->circleShadows[i].active = 0;
			}
		}
		constBuff->Unmap(0, nullptr);
	}
	
}

void Light::SetPointLightActive(int index, bool active)
{
	assert(0 <= index && index < PointLightNum);
	pointLight[index].SetActive(active);
}

void Light::SetPointLightPos(int index, const XMFLOAT3& lightpos)
{
	assert((0 <= index) && (index < PointLightNum));

	pointLight[index].SetLightPos(lightpos);
	dirty = true;
}

void Light::SetPointLightColor(int index, const XMFLOAT3& lightColor)
{
	assert((0 <= index) && (index < PointLightNum));
	pointLight[index].SetLightColor(lightColor);
	dirty = true;
}

void Light::SetPointLightAtten(int index, const XMFLOAT3& lightAtten)
{
	assert((0 <= index) && (index < PointLightNum));
	pointLight[index].SetLightAtten(lightAtten);
	dirty = true;
}

void Light::SetSpotLightActive(int index, bool active)
{
	assert(0 <= index && index < SpotLightNum);
	spotLights[index].SetActive(active);
}

void Light::SetSpotLightDir(int index, const XMVECTOR& lightDir)
{
	assert(0 <= index && index < SpotLightNum);
	spotLights[index].SetLightDir(lightDir);
	dirty = true;
}

void Light::SetSpotLightPos(int index, const XMFLOAT3& lightPos)
{
	assert(0 <= index && index < SpotLightNum);
	spotLights[index].SetLightPos(lightPos);
	dirty = true;
}

void Light::SetSpotLightColor(int index, const XMFLOAT3& lightColor)
{
	assert(0 <= index && index < SpotLightNum);
	spotLights[index].SetLightColor(lightColor);
	dirty = true;
}

void Light::SetSpotLightAtten(int index, const XMFLOAT3& lightAtten)
{
	assert(0 <= index && index < SpotLightNum);
	spotLights[index].SetLightAtten(lightAtten);
	dirty = true;
}

void Light::SetSpotLightAngle(int index, const XMFLOAT2& lightFactorAngle)
{
	assert(0 <= index && index < SpotLightNum);
	spotLights[index].SetLightFactorAngle(lightFactorAngle);
	dirty = true;
}

void Light::SetCircleShadowActive(int index, bool active)
{
	assert(0 <= index && index <= CircleShadowNum);
	circleShadows[index].SetActive(active);
}

void Light::SetCircleShadowCasterPos(int index, const XMFLOAT3& casterPos)
{
	assert(0 <= index && index <= CircleShadowNum);
	circleShadows[index].SetCasterPos(casterPos);
	dirty = true;
}

void Light::SetCircleShadowDir(int index, XMVECTOR& lightDir)
{
	assert(0 <= index && index <= CircleShadowNum);
	circleShadows[index].SetDir(lightDir);
	dirty = true;
}

void Light::SetCircleShadowDistanceCasterLight(int index, float distanceCasterLight)
{
	assert(0 <= index && index <= CircleShadowNum);
	circleShadows[index].SetDistanceCasterLight(distanceCasterLight);
	dirty = true;
}

void Light::SetCircleShadowAtten(int index, const XMFLOAT3 lightAtten)
{
	assert(0 <= index && index <= CircleShadowNum);
	circleShadows[index].SetAtten(lightAtten);
	dirty = true;
}

void Light::SetCircleShadowFactorAngle(int index, const XMFLOAT2& lightfactorAngle)
{
	assert(0 <= index && index <= CircleShadowNum);
	circleShadows[index].SetFactorAngle(lightfactorAngle);
	dirty = true;
}

void Light::SetLightDir(const XMVECTOR& lightDir)
{
	this->lightdir = XMVector3Normalize(lightDir);
	dirty = true;
}

void Light::SetLightColor(const XMFLOAT3& lightColor)
{
	this->lightcolor = lightColor;
	dirty = true;
}

void Light::Update()
{
	if (dirty)
	{
		TransConstBuffer();
		dirty = false;
	}
}

void Light::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff->GetGPUVirtualAddress());
}

Light* Light::Create()
{
	Light *instance = new Light();
	instance->CreateBuff();
	return instance;
}
