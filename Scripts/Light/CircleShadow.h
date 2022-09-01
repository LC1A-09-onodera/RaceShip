#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class CircleShadow
{
public:
	struct ConstBufferData
	{
		XMVECTOR dir;
		XMFLOAT3 casterPos;
		float distanceCasterLight;
		XMFLOAT3 atten;
		float pad3;
		XMFLOAT2 factorAngleCos;
		unsigned int active;
		float pad4;
	};
private:
	XMVECTOR dir = {1, 0, 0, 0};
	//キャスターとライトの距離
	float distanceCasterLight = 100.0f;
	//キャスターの座標(ワールド座標)
	XMFLOAT3 casterPos = {0, 0, 0};
	XMFLOAT3 atten = {0.5f, 0.6f, 0.0f};
	XMFLOAT2 factorAngleCos = {0.2f, 0.5f};
	bool active = false;
public:
	inline void SetDir(const XMVECTOR &Dir){this->dir = Dir;}
	inline const XMVECTOR &GetDir(){return dir;}
	inline void SetCasterPos(const XMFLOAT3 &CasterPos){this->casterPos = CasterPos;}
	inline const XMFLOAT3 &GetCasterPos(){return casterPos;}
	inline void SetDistanceCasterLight(float DistanceCasterLight){this->distanceCasterLight = DistanceCasterLight;}
	inline float GetDistanceCasterLight(){return distanceCasterLight;}
	inline void SetAtten(const XMFLOAT3 &Atten) {this->atten = Atten;}
	inline const XMFLOAT3 GetAtten(){return atten;}
	inline void SetFactorAngle(const XMFLOAT2 &factorAngle){this->factorAngleCos.x = cosf(XMConvertToRadians(factorAngle.x));
															this->factorAngleCos.y = cosf(XMConvertToRadians(factorAngle.y));
	}
	inline const XMFLOAT2 &GetFactorAngleCos(){return factorAngleCos;}
	inline void SetActive(bool isActive){this->active = isActive;}
	inline bool isActive(){return active;}
};