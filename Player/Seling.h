#pragma once
#include "../3DModel/Model.h"
#include "../BaseDirectX/DX12operator.h"
#include "../Enemy/selingEnemy.h"
#include "../Rewired/Rewired.h"

class ShieldModel : public Model
{
public:
	
	bool InitializeGraphicsPipeline(BaseDirectX& baseDirectX, HLSLShader& shader);
};

class Seling
{
	
	int nowNumber;
	XMFLOAT3 frontDirection = { 0.0f ,0.0f , 0.0f };
	float angle;

	bool isMoveForce;
	XMFLOAT3 addForce = { 0.0f ,0.0f , 0.0f};
	XMFLOAT3 subForce = { 0.0f ,0.0f , 0.0f};
	XMFLOAT3 maxForce = { 0.0f ,0.0f , 0.0f};

	Model shotModel;
	XMFLOAT3 shotPos = { 0.0f ,0.0f , 0.0f };
	bool isShot;

	ShieldModel shieldModel;
	bool isShield;
	XMFLOAT3 shieldPos = { 0.0f ,0.0f , 0.0f};
	XMFLOAT3 scaleStart = { 0.0f ,0.0f , 0.0f };
	XMFLOAT3 scaleEnd = { 0.0f ,0.0f , 0.0f };
	float easeTime = 0;
	int shieldTime = 0;
	const int shieldMaxTime = 180;

	bool isGoal = false;

	//Enemy enemy;

	const float addShieldRotaion = 30.0f;
	const float shieldR = 2.2f;
	const float addForcePower = 0.02f;
	const float MaxForce = 1.0f;
public:
	Model seling;
	void AddForce(XMFLOAT3& force);
	XMFLOAT3 GetDirection()
	{
		return Normalize(frontDirection);
	}
	bool GetIsGoal() { return isGoal; }
	void ForceUpdate();
	void LoadModel(BaseDirectX &baseDirectX);
	void Init();
	void Update();
	void Draw(BaseDirectX& baseDirectX, bool isRCamera = false);

	void ForceAttach();
	void Move();

	//弾が出る
	void ShotInit();
	void ShotUpdate();
	void ShotInitAndUpdate();

	//シールドが出る
	void ShieldInit();
	void ShieldUpdate();
	void ShieldInitAndUpdate();

	void HitWall();
	void HitGoal();
};