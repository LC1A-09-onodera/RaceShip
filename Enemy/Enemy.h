#pragma once
#include "../3DModel/Model.h"
#include <DirectXMath.h>

using namespace DirectX;
class King;

class EnemyModel : public Model
{

};

class EnemyBase
{
private:
	//
	EnemyModel enemy;
	int hp;
	//死んでたらtrue
	bool isDead;
	//爆風受けたらtrue
	bool isWind;
	//動く速度
	float moveSpeed;
	//キングの方向
	XMFLOAT3 kingDirection;
	//爆風の押し出しPower
	XMFLOAT3 windDirection;
public:
	EnemyModel SetModel(EnemyModel model){enemy = model;}
	void SetHP(int HP){this->hp = HP;}
	void SetIsDead(bool isDead){this->isDead = isDead;}
	void SetIsWind(bool isWind){ this->isWind = isWind;}
	void SetMoveSpeed(float moveSpeed){ this->moveSpeed = moveSpeed;}
	void SetKingDirection(XMFLOAT3 direction){this->kingDirection = direction;}
	void SetWindDirection(XMFLOAT3 windDirection){this->windDirection = windDirection;}
	int GetHP(){return hp;}
	bool GetIsDead(){return isDead;}
	bool GetIsWind(){return isWind;}
	float GetIsMoveSpeed(){return moveSpeed;}
	XMFLOAT3 GetKingDirection(){return kingDirection;}
	XMFLOAT3 GetWindDirection(){return windDirection;}
	EnemyModel GetModel(){return enemy;}
};
