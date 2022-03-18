#pragma once
#include "../3DModel/Model.h"
#include "../DX12operator.h"

class KingModel : public Model
{

};

class King
{
private:
	KingModel king;
	int HP;
	XMFLOAT3 windDirection;
	XMFLOAT3 enemyDirection;
	bool isWind;
	bool isEnemyHit;
	const float resistForce = 1.0f;
	const float resistMinForce = 1.1f;
public:
	void LoadModel();
	KingModel GetModel() { return king; }
	XMFLOAT3 GetPosition(){return ConvertXMVECTORtoXMFLOAT3(this->king.each.position); }
	void SetPosition(XMFLOAT3 &position);
	void SetIsWind(bool flag){ isWind = flag; }
	void SetIsEnemyHit(bool flag) { isEnemyHit = flag; }
	void SetEnemyDirection(XMFLOAT3 &direction) { enemyDirection = direction; }
	bool GetIsEnemyhit(){ return isEnemyHit; }
	int GetHP(){ return HP; }

	void Init(int HP = 10);
	void Update();
	void Draw();

	void AddWindForce();
	void AddEnemyForce();

};