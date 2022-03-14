#include "Bomb.h"
#include "../DX12operator.h"
#include "../Shader/ShaderManager.h"
Bomb::Bomb()
{
}

Bomb::~Bomb()
{
}

void Bomb::Init()
{
	bombObject.CreateModel("Block", ShaderManager::playerShader);
	blastObject.CreateModel("Block", ShaderManager::playerShader);
}

void Bomb::Update()
{
	if (data.isAlive)
	{
		BombUpdate();
	}
	if (data.isBlast)
	{
		BlastUpdate();
	}
	bombObject.position = data.pos;
	blastObject.position = data.pos;
}

void Bomb::Finailize()
{
}

void Bomb::Draw()
{
}

bool Bomb::Shot(DirectX::XMFLOAT3 angle, DirectX::XMFLOAT3 pos)
{
	//”š’e‚ª¶‚«‚Ä‚¢‚é‚È‚ç
	if (data.isAlive) return false;

	//”š’e‚Ìî•ñ‚ğ’Ç‰Á
	data.pos = ConvertXMFLOAT3toXMVECTOR(angle);
	data.isAlive = true;
	data.bombAngle = ConvertXMFLOAT3toXMVECTOR(angle);
	return true;
}

XMFLOAT3 Bomb::EnemyBombCollision(EnemyBase enemyData)
{
	//“G‚Æ“–‚½‚Á‚Ä‚¢‚é‚©‚Ì”»’è
	float bombEnemyDistance = 
	//”š’e‚©‚ç“G‚ÉŒü‚©‚Á‚Ä‚Ì•ûŒüŒvZ

	//“G‚Æ”š’e‚Æ‚Ì‹——£ŒvZ

	//”š”­‚Ì”ÍˆÍ‚Æ”š•—‚Ì”ÍˆÍ‚Ì’·‚³‚ğ”äŠr

	//”ä—¦‚É‰‚¶‚Ä‰Ÿ‚µ–ß‚µ‚Ì—Í‚Ì‘å‚«‚³‚ğ•ÏX

	//Œü‚«‚Æ—Í‚Ì‘å‚«‚³‚ğæZ‚µ‚ÄƒxƒNƒgƒ‹‚É‚µ‚ÄŠµ«
	DirectX::XMVECTOR blastVector = {};

	return XMFLOAT3();
}

void Bomb::BombUpdate()
{
	DirectX::XMVECTOR moveSpeed = (data.bombAngle * data.bombSpeed);
	data.pos += moveSpeed;
}

void Bomb::BlastUpdate()
{
}
