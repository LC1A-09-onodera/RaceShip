#pragma once
#include "../3DModel/Model.h"
#include "../DX12operator.h"
#include "../Enemy/selingEnemy.h"

class ShieldModel : public Model
{
public:
	
	bool InitializeGraphicsPipeline(HLSLShader& shader);
	void CreateModel(const char* name, HLSLShader& shader, bool smoothing);
};

class Seling
{
	Model seling;
	int nowNumber;
	XMFLOAT3 frontDirection;
	float angle;

	bool isMoveForce;
	XMFLOAT3 addForce;
	XMFLOAT3 subForce;
	XMFLOAT3 maxForce;

	Model shotModel;
	XMFLOAT3 shotPos;
	bool isShot;

	ShieldModel shieldModel;
	bool isShield;
	XMFLOAT3 shieldPos;

	Enemy enemy;

public:
	void AddForce(XMFLOAT3& force)
	{
		if (this->addForce.x < maxForce.x && this->addForce.x > -maxForce.x)
		{
			this->addForce.x = this->addForce.x + force.x;
		}
		if (this->addForce.y < maxForce.y && this->addForce.y > -maxForce.y)
		{
			this->addForce.y = this->addForce.y + force.y;
		}
		if (this->addForce.z < maxForce.z && this->addForce.z > -maxForce.z)
		{
			this->addForce.z = this->addForce.z + force.z;
		}
		isMoveForce = true;
	}
	XMFLOAT3 GetDirection()
	{
		return Normalize(frontDirection);
	}
	void ForceUpdate();
	void LoadModel();
	void Init();
	void Update();
	void Draw();

	void ForceAttach();
	void Move();

	//�㉺���铮��
	void VerticalWaterResist();
	void HolizontalWaterResist();

	void CreateSimpleHarmonicMotion();

	//�e���o��
	void ShotInit();
	void ShotUpdate();

	//�V�[���h���o��
	void ShieldInit();
	void ShieldUpdate();
};