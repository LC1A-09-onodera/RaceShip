#pragma once
//#include "../3DModel/Model.h"
#include "../sampleObject/sampleObject.h"
#include "../Collition/CollisionPrimitive.h"
#include "../Sound/Sound.h"

using namespace DirectX;

const int MAX_ACTIVE_COUNT = 300;
const int MAX_INVICIBLE_COUNT = 120;
const float MAX_SPEED = 5.0f;

static XMFLOAT3 VecNormaliz(XMFLOAT3& vector)
{
	float leg = sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	vector.x = vector.x / leg;
	vector.y = vector.y / leg;
	vector.z = vector.z / leg;
	return vector;
}

class Player final
{
private:
	Player();
	~Player();

	XMFLOAT3 pos;			//プレイヤーの座標
	XMFLOAT3 vec3;			//向いている方向（正規化済）
	int activeCount;		//行動不能カウント
	int invincibleCount;	//無敵カウント
	bool isActive;			//行動できるかどうか
	bool isInvincible;		//無敵かどうか
	bool isShoot;			//射撃中かどうか（弾があるか）
	bool isDetonating;		//起爆したかどうか

public:
	Player(const Player& obj) = delete;
	Player& operator=(const Player& obj) = delete;
	static Player* GetPlayer();
	//初期化
	void Init();
	//更新
	void Update(bool isBombAlive,bool isHit);
	//描画
	void Draw();

	XMFLOAT3 GetPos() { return pos; }				//プレイヤーの座標
	XMFLOAT3 GetVec3() { return vec3; }				//向いている方向（正規化済） 
	bool IsActive() { return isActive; }			//行動できるかどうか
	bool IsInvincible() { return isInvincible; }	//無敵かどうか
	bool IsShoot() { return isShoot; }				//射撃中かどうか
	//void EndShoot() { isShoot = false; }			//弾が画面外か起爆されたか敵と当たった場合に呼び出す
	bool IsDetonating() { return isDetonating; }	//起爆したかどうか


	XMVECTOR oldPlayerPos;
	Box2D playerCollision;
	SampleObject player;
	int particleTime;
	bool isParticle;
};