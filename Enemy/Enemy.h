#pragma once
#include "../3DModel/Model.h"
#include <DirectXMath.h>

using namespace DirectX;

class EnemyBase
{
public:
	int hp;
	bool isDead;
	//爆風受けたらtrue
	bool isBless;
	float moveSpeed;
	XMFLOAT3 direction;
	//爆風の押し出しPower
	XMFLOAT3 blessDirection;
};

class EnemeyProsess
{
public:
	static void Update(EnemyBase &enemy);
	static void Init(EnemyBase &enemy);
	static void SetKingDirection(EnemyBase &enemy);
	static void SetIsBless(EnemyBase &enemy, bool flag);
};
