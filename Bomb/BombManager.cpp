#include "BombManager.h"

namespace
{
	const int bombsMaxCount = 10;
}
BombManager::BombManager()
{
	bombs.resize(bombsMaxCount);
}

BombManager::~BombManager()
{
}

void BombManager::Init()
{
	for (int i = 0; i < bombs.size(); i++)
	{
		bombs[i].Init();
	}
}

void BombManager::Update()
{
	for (int i = 0; i < bombs.size(); i++)
	{
		bombs[i].Update();
	}
}

void BombManager::Finalize()
{
	for (int i = 0; i < bombs.size(); i++)
	{
		bombs[i].Finailize();
	}
}

void BombManager::Draw()
{
	for (int i = 0; i < bombs.size(); i++)
	{
		bombs[i].Draw();
	}
}

void BombManager::Shot(DirectX::XMFLOAT3 angle, DirectX::XMFLOAT3 pos)
{
	bool isShot = false;
	const int Nothing = 100;
	int bombNumber = Nothing;
	for (int i = 0; i < bombs.size(); i++)
	{
		if (bombs[i].GetIsAlve())
		{
			return;
		}

		//”­ŽË‚Å‚«‚é‚©
		if (!bombs[i].GetIsAlve() && !bombs[i].GetIsExplosion())
		{
			bombNumber = i;
		}

	}
	if (bombNumber != Nothing)
	{
		bombs[bombNumber].Shot(angle, pos);
	}
}

bool BombManager::GetBombAlive()
{
	for (int i = 0; i < bombs.size(); i++)
	{
		if (bombs[i].GetIsAlve())
		{
			return true;
		}
	}
	return false;
}

void BombManager::enemyCollision(EnemyBase &data)
{
	for (int i = 0; i < bombs.size(); i++)
	{
		bombs[i].EnemyBombCollision(data);
	}
}

void BombManager::PlayerCollision(XMFLOAT3 pos, float radius)
{
	float power = 0.0f;
	for (int i = 0; i < bombs.size(); i++)
	{
	bombs[i].PlayerBlastCollision(pos, radius);
	}
}
