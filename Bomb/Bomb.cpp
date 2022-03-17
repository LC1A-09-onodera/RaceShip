#include "Bomb.h"
#include "../DX12operator.h"
#include "../Shader/ShaderManager.h"

namespace
{
	const int explosionTimerMax = 30;
}
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
	if (data.isExplosion)
	{
		BlastUpdate();
	}

	bombObject.Update();
	blastObject.Update();
}

void Bomb::Finailize()
{
}

void Bomb::Draw()
{
	if (data.isAlive)
	{
		Draw3DObject(bombObject);
	}
	if (data.isExplosion)
	{
		Draw3DObject(blastObject);
	}
}

bool Bomb::Shot(DirectX::XMFLOAT3 angle, DirectX::XMFLOAT3 pos)
{
	//”š’e‚ª¶‚«‚Ä‚¢‚é‚È‚ç
	if (data.isAlive) return false;

	//”š’e‚Ìî•ñ‚ğ’Ç‰Á
	data.pos = ConvertXMFLOAT3toXMVECTOR(pos);
	data.isAlive = true;
	data.bombAngle = ConvertXMFLOAT3toXMVECTOR(angle);
	return true;
}

void Bomb::EnemyBombCollision(EnemyBase &enemyData)
{
	//“G‚ÌÀ•W
	XMVECTOR enemyPosition = ConvertXMFLOAT3toXMVECTOR(enemyData.GetPosition());

	//”š’e–{‘Ì‚ÆÚG‚µ‚Ä‚¢‚é‚©‚Ì”»’è
	bool IsBlast = BombCollision(enemyPosition, 1);

	//”š’e‚ÆÚG‚µ‚Ä‚¢‚½‚ç”š”­‚·‚é
	if (IsBlast)
	{
	//”š’e–{‘Ì‚ÆÚG‚µ‚Ä‚¢‚é‚©‚Ì”»’è
		BombCollision(enemyPosition, 0);
		Explosion();
	}


	//”š•—‚ÌƒxƒNƒgƒ‹
	XMFLOAT3 blastPower;

	//”š•—‚É“–‚½‚Á‚½‚©ƒtƒ‰ƒO
	bool IsBlastHit = BlastCollision(enemyPosition, 0, &blastPower);


	//“–‚½‚Á‚½î•ñ‚ğŠeƒf[ƒ^‚É“ü‚ê‚é
	if (IsBlastHit)
	{
		enemyData.SetIsWind(IsBlastHit);
		enemyData.SetWindDirection(blastPower);
	}
}



void Bomb::BombUpdate()
{
	DirectX::XMVECTOR moveSpeed = (data.bombAngle * data.bombSpeed);
	data.pos += moveSpeed;

	bombObject.each.position = data.pos;
}

void Bomb::BlastUpdate()
{
	data.blastTimer++;
	blastObject.each.position = data.pos;

	blastObject.each.scale = (blastObject.each.scale + XMFLOAT3{ 1.0f, 1.0f, 1.0f });
	if (data.blastTimer >= explosionTimerMax)
	{
		data.isExplosion = false;
	}
}

bool Bomb::BombCollision(const XMVECTOR &pos, const float &radius)
{
	//”š’e‘¶İ‚ª‚µ‚Ä‚¢–³‚©‚Á‚½‚ç‘ŠúƒŠƒ^[ƒ“
	if (!data.isAlive)return false;

	//”š’e‚©‚ç“G‚Ü‚Å‚ÌƒxƒNƒgƒ‹‚ğŒvZ
	XMVECTOR distance = (pos - data.pos);
	//“G‚Æ”š’e‚Æ‚Ì‹——£ŒvZ
	float bombEnemyLenght = XMVector3Length(distance).m128_f32[0];

	if (bombEnemyLenght <= 0)
	{
		return false;
	}
	//“G‚Æ”š’e‚ª“–‚½‚Á‚Ä‚¢‚é‚©“–‚½‚Á‚Ä‚¢‚é‚©‚Ì”»’è
	if ((data.bombRadius + radius) <= bombEnemyLenght)
	{//”š’e‚ªÚG‚µ‚Ä‚¢‚È‚©‚Á‚½
		return false;
	}
	else
	{
		return true;
	}
}

bool Bomb::BlastCollision(const XMVECTOR &pos, const float &radius, XMFLOAT3 *blastPower)
{
	//”š”­‚µ‚Ä‚¢‚È‚©‚Á‚½‚ç
	if (!data.isExplosion)return false;

	//”š’e‚©‚ç“G‚Ü‚Å‚ÌƒxƒNƒgƒ‹‚ğŒvZ
	XMVECTOR distance = (pos - data.pos);
	//“G‚Æ”š’e‚Æ‚Ì‹——£ŒvZ
	float bombEnemyLenght = XMVector3Length(distance).m128_f32[0];

	//“G‚Æ“–‚½‚Á‚Ä‚¢‚é‚©‚Ì”»’è
	if (data.blastRadius <= bombEnemyLenght)
	{//”š•—‚ª“G‚ÆÚG‚µ‚Ä‚¢‚È‚©‚Á‚½‚ç‘¬“xƒ[ƒ‚ğ•Ô‚·
		return false;
	}


	//”š’e‚©‚ç“G‚ÉŒü‚©‚Á‚Ä‚Ì•ûŒüŒvZ
	XMVECTOR blastAngle = XMVector3Normalize(distance);
	//”š”­‚Ì”ÍˆÍ‚Æ”š•—‚Ì”ÍˆÍ‚Ì’·‚³‚ğ”äŠr
	float power = 1.0f - (bombEnemyLenght / data.blastRadius);
	//”ä—¦‚É‰‚¶‚Ä‰Ÿ‚µ–ß‚µ‚Ì—Í‚Ì‘å‚«‚³‚ğ•ÏX
	power *= data.blastPower;
	//Œü‚«‚Æ—Í‚Ì‘å‚«‚³‚ğæZ‚µ‚ÄƒxƒNƒgƒ‹‚É‚µ‚ÄŠµ«
	DirectX::XMVECTOR blastVector = blastAngle * power;
	//“–‚½‚Á‚Ä‚¢‚é”»’è
	if (blastPower)
	{
		*blastPower = ConvertXMVECTORtoXMFLOAT3(blastVector);
	}
	return true;
}

void Bomb::Explosion()
{
	data.blastTimer = 0;
	data.isAlive = false;
	data.isExplosion = true;
}
