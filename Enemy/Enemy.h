#pragma once
#include "../3DModel/Model.h"
#include <DirectXMath.h>
#include "../DX12operator.h"

using namespace DirectX;

enum class EnemyType
{
	NONE,
};

enum class Target
{
	KING,
	PLAYER,
	CROWN,
};

class EnemyModel : public Model
{

};

class EnemyModels
{
public:
	static EnemyModel baseEnemy;
	static void LoadModels();
};

class EnemyBase
{
private:
	//
	EnemyModel enemy;
	EachInfo sample;
	EnemyType type;
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
	//
	Target target;
	const int MaxHP = 1;
	const float MaxMoveSpeed = 1.0f;
public:
	void SetMesh(EnemyModel &model) { enemy = model; }
	void SetHP(int HP) { this->hp = HP; }
	void SetIsDead(bool isDead) { this->isDead = isDead; }
	void SetIsWind(bool isWind) { this->isWind = isWind; }
	void SetMoveSpeed(float moveSpeed) { this->moveSpeed = moveSpeed; }
	void SetKingDirection(XMFLOAT3 direction) { this->kingDirection = direction; }
	void SetWindDirection(XMFLOAT3 windDirection) { this->windDirection = windDirection; }
	int GetHP() { return hp; }
	bool GetIsDead() { return isDead; }
	bool GetIsWind() { return isWind; }
	float GetIsMoveSpeed() { return moveSpeed; }
	XMFLOAT3 GetKingDirection() { return kingDirection; }
	XMFLOAT3 GetWindDirection() { return windDirection; }
	XMFLOAT3 GetPosition() { return ConvertXMVECTORtoXMFLOAT3(enemy.each.position); }
	EnemyModel GetModel() { return enemy; }

	void Init();
	void Update();
	void Draw();

	void SetRandomPosition();
	void SetAlive();
	void UpdateKingDirection(XMFLOAT3 &kingPos);
};

class Enemys
{
public:
	static list<EnemyBase> enemys;
	static void AddEnemy(EnemyType type);
	static void Update();
	static void Draw();
};