#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class PointLight
{
private:

public:
	struct ConstBuffData
	{
		XMFLOAT3 lightpos;
		float pad1;
		XMFLOAT3 lightcolor;
		float pod2;
		XMFLOAT3 lighttatten;
		unsigned int active;
	};
	inline void SetLightPos(const XMFLOAT3 &lightPos){this->lightpos = lightPos;}
	inline const XMFLOAT3 &GetLightPos(){return lightpos;}
	inline void SetLightColor(const XMFLOAT3 &lightColor){this->lightcolor = lightColor;}
	inline const XMFLOAT3 &GetColor(){return lightcolor;}
	inline void SetLightAtten(const XMFLOAT3 &lightAtten){this->lighttatten = lightAtten;}
	inline const XMFLOAT3 &GetAtten(){return lighttatten;}
	inline void SetActive(bool Active){this->active = Active;}
	inline bool isActive(){return active;}
private:
	//座標
	XMFLOAT3 lightpos = { 0, 0, 0 };
	//ライトの色
	XMFLOAT3 lightcolor = { 1, 1, 1 };
	//距離減衰係数
	XMFLOAT3 lighttatten = { 1.0f, 1.0f, 1.0f};
	bool active = false;
};