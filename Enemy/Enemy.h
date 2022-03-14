#pragma once
#include "../3DModel/Model.h"
#include <DirectXMath.h>

using namespace DirectX;

struct Bullet;

enum EnemyAction
{
	ARMATTACK = 1, FOOTATTACK, MOVE, DOWN, STAY, DIE , ARMATTACK2, BIGBLOCK, RANDOM
};

enum CollisionPos
{
	RARM = 21848, RARMJOINT = 21851, HEAD = 21854, BODY = 21857, LARMJOINT = 21860, LARM = 21863, LHIP = 21866, HIP = 21867, RHIP = 21872, RFOOT = 21875, LFOOT = 21878, LFOOTJOINT = 21881, RFOOTJOINT = 21884, WEEKPOINT1 = 2429, 
};

//struct Bullet
//{
//	Model bullet;
//	XMFLOAT3 pos;
//	XMFLOAT3 direction;
//	bool isActive;
//	int life;
//};

struct Collider
{
	Model collider;
	float ypos;
	float lenght;
	bool isDirection;
};

class Enemy
{
public:
	
public:
	//初期化
	static void Init();
	//更新
	static void Update();
	//描画
	static void Draw();
	//再スタート時に使うような関数
	static void ReSet();

	static XMFLOAT3 Lerp(const XMFLOAT3 &start, XMFLOAT3 &end, const float t);
};