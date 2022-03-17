#pragma once
#include "../3DModel/Model.h"
#include <DirectXMath.h>
#include "../DX12operator.h"

using namespace DirectX;
class King;

enum class EnemyType
{
	NONE,
	SUPER,
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
	static EnemyModel superEnemy;
	static void LoadModels();
};

class EnemyBase
{
//private:
protected:
	//
	EnemyModel enemy;
	//イージングスタート位置
	XMFLOAT3 startPos;
	//イージングの終了位置
	XMFLOAT3 endPos;
	//イージングタイム
	float easeTime;
	//イージングタイムの増加率
	float easeAddTime;
	//
	EachInfo each;

	EnemyType type;
	int hp;
	//死んでたらtrue
	bool isDead;
	//爆風受けたらtrue
	bool isWind;
	//動く速度
	float moveSpeed;
	//
	float weight;
	//キングの方向
	XMFLOAT3 kingDirection;
	//爆風の押し出しPower
	XMFLOAT3 windDirection;
	//プレイヤーとの押し出し
	XMFLOAT3 playerPushDirection;
	//
	Target target;
	const int MaxHP = 1;
	const float MaxMoveSpeed = 0.1f;
	const float MaxWeight = 1.0f;
public:
	void SetMesh(EnemyModel &model) { enemy = model; }
	void SetHP(int HP) { this->hp = HP; }
	void SetIsDead(bool isDead) { this->isDead = isDead; }
	void SetIsWind(bool isWind) { this->isWind = isWind; }
	void SetWeight(float weight) { this->weight = weight; }
	void SetMoveSpeed(float moveSpeed) { this->moveSpeed = moveSpeed; }
	void SetKingDirection(XMFLOAT3 direction) { this->kingDirection = direction; }
	void SetWindDirection(XMFLOAT3 windDirection) { this->windDirection = windDirection; }
	void SetPlayerPushDirection(XMFLOAT3 playerPushDirection) { this->playerPushDirection = playerPushDirection; }
	int GetHP() { return hp; }
	bool GetIsDead() { return isDead; }
	bool GetIsWind() { return isWind; }
	bool GetWeight() { return weight; }
	float GetIsMoveSpeed() { return moveSpeed; }
	XMFLOAT3 GetKingDirection() { return kingDirection; }
	XMFLOAT3 GetWindDirection() { return windDirection; }
	XMFLOAT3 GetPosition() { return ConvertXMVECTORtoXMFLOAT3(each.position); }
	EnemyModel GetModel() { return enemy; }

	void Init();
	void Update(King &king);
	void Draw();
	//ランダムポジションに生成
	void SetRandomPosition();
	//Init時にFlagをセットする
	void SetAlive();
	//ヘイトの方向を取得する
	void UpdateKingDirection(XMFLOAT3 &kingPos);
	void HitDethLine();
};

class SuperEnemy : public EnemyBase
{
private:
	const int MaxHP = 2;
	const float MaxMoveSpeed = 0.5f;
	const float MaxWeight = 2.0f;
	//static const int dethLine = 25;
public:
	void Init();
	void SetAlive();
};

class Enemys
{
public:
	static list<EnemyBase> enemys;
	static list<list<EnemyBase>::iterator> deleteEnemys;
	static void AddEnemy(EnemyType type);
	static void DeathEnemy(EnemyBase &enemy);
	static void Update(King &king);
	static void Draw();
};