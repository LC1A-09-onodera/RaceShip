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
	each.CreateConstBuff0();
	each.CreateConstBuff1();
	SetAlive();
}
void EnemyBase::Update()
{
	//”š•—‚ðŽó‚¯‚Ä‚¢‚È‚¢
	if (!isWind)
	{
		kingDirection = king.GetPosition() - GetPosition();
		each.position += ConvertXMFLOAT3toXMVECTOR(Normalize(kingDirection) * moveSpeed);

	}
	else
	{

	}
}
void EnemyBase::Draw()
{
	enemy.Update(&each);
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
	if (rand() % 2 == 0)
	{
		if (rand() % 2)
		{
			each.position.m128_f32[0] = rand() % 10 + 30;
		}
		else
		{
			each.position.m128_f32[0] = -(rand() % 10) - 30;
		}
		//ã‰º‚Ì’²®
		each.position.m128_f32[2] = rand() % 30 - 15;
	}
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
