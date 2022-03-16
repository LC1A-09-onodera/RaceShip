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
	for (int i = 0; i < bombs.size(); i++)
	{
		//”­ŽË‚Å‚«‚é‚©
		bool isShot = (!bombs[i].GetIsAlve() && !bombs[i].GetIsExplosion());
		if (isShot)
		{
			bombs[i].Shot(angle, pos);
			break;
		}
	}
}
