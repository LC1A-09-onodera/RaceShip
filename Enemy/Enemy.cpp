#include "Enemy.h"
#include "../Player/Player.h"
#include "../Particle/Particle3D.h"
#include "../Sound/Sound.h"
#include "../King/King.h"
#include "../WindowsAPI/WinAPI.h"

void EnemyBase::Init()
{

	SetAlive();
}
void EnemyBase::Init(XMFLOAT3 position, EnemyType enemyType, int hp, float moveSpeed)
{
	if (enemyType == EnemyType::NONE)
	{

		type = enemyType;
	}
	SetAlive();
}
void EnemyBase::Update()
{

}
void EnemyBase::Draw()
{
	enemy.Update();
	Draw3DObject(enemy);
}
void EnemyBase::SetAlive(int hp, float moveSpeed)
{
	SetRandomPosition();
	this->hp = hp;
	this->moveSpeed = moveSpeed;
	isDead = false;
	isWind = false;
	kingDirection = { 1, 0, 0 };
	windDirection = { 0, 0, 0 };
}
void EnemyBase::SetRandomPosition()
{
	//左右から出てくる
	if (rand() % 2 == 0)
	{
		if (rand() % 2 == 0)
		{
			enemy.position.m128_f32[0] = WindowsAPI::window_width + 10;
		}
		else
		{
			enemy.position.m128_f32[0] = -WindowsAPI::window_width - 10;
		}
		//上下の調整
		enemy.position.m128_f32[2] = (rand() % WindowsAPI::window_height) - (WindowsAPI::window_height / 2);
	}
	//上下から出てくる
	else
	{
		if (rand() % 2 == 0)
		{
			enemy.position.m128_f32[2] = WindowsAPI::window_height + 10;
		}
		else
		{
			enemy.position.m128_f32[2] = -WindowsAPI::window_height - 10;
		}
		//左右の調整
		enemy.position.m128_f32[0] = (rand() % WindowsAPI::window_width) - (WindowsAPI::window_width / 2);
	}
}
