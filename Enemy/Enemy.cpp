#include "Enemy.h"
#include "../Player/Player.h"
#include "../Particle/Particle3D.h"
#include "../Sound/Sound.h"
#include "../WindowsAPI/WinAPI.h"
#include "../King/King.h"
#include "../Shader/ShaderManager.h"

EnemyModel EnemyModels::baseEnemy;
list<EnemyBase> Enemys::enemys;
void EnemyBase::Init()
{
	SetMesh(EnemyModels::baseEnemy);
	sample.CreateConstBuff0();
	sample.CreateConstBuff1();
	//enemy.CreateModel("maru", ShaderManager::playerShader);
	SetAlive();
}
void EnemyBase::Update()
{
	//”š•—‚ðŽó‚¯‚Ä‚¢‚È‚¢
	if (!isWind)
	{
		sample.position += ConvertXMFLOAT3toXMVECTOR(kingDirection);
	}
	else
	{

	}
}
void EnemyBase::Draw()
{
	enemy.each = sample;
	enemy.Update();
	Draw3DObject(enemy);
}
void EnemyBase::SetAlive()
{
	SetRandomPosition();
	this->hp = MaxHP;
	this->moveSpeed = MaxMoveSpeed;
	isDead = false;
	isWind = false;
	kingDirection = { 0.1f, 0, 0 };
	windDirection = { 0, 0, 0 };
}
void EnemyBase::UpdateKingDirection(XMFLOAT3& kingPos)
{
	XMFLOAT3 result;
	result = kingPos - GetPosition();
	kingDirection = Normalize(result);
}
void EnemyBase::SetRandomPosition()
{
	//¶‰E‚©‚ço‚Ä‚­‚é
	/*if (rand() % 2 == 0)
	{*/

		sample.position.m128_f32[0] = -20;
	
		//ã‰º‚Ì’²®
		sample.position.m128_f32[2] = rand() % 30 - 15;
	//}
	//ã‰º‚©‚ço‚Ä‚­‚é
	//else
	//{
	//	if (rand() % 2 == 0)
	//	{
	//		enemy.position.m128_f32[2] = 10;
	//	}
	//	else
	//	{
	//		enemy.position.m128_f32[2] = -10;
	//	}
	//	//¶‰E‚Ì’²®
	//	enemy.position.m128_f32[0] = 30;
	//}
}

void Enemys::AddEnemy(EnemyType type)
{
	if (type == EnemyType::NONE)
	{
		//ŽG‹›“G
		EnemyBase enemy;
		enemy.Init();
		enemys.push_back(enemy);
	}
	else
	{

	}
}

void Enemys::Update()
{
	auto itr = enemys.begin();
	for (; itr != enemys.end(); ++itr)
	{
		itr->Update();
	}
}

void Enemys::Draw()
{
	auto itr = enemys.begin();
	for (; itr != enemys.end(); ++itr)
	{
		itr->Draw();
	}
}

void EnemyModels::LoadModels()
{
	baseEnemy.CreateModel("maru", ShaderManager::playerShader);
}
