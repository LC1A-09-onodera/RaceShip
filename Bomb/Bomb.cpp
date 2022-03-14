#include "Bomb.h"
#include "../DX12operator.h"
Bomb::Bomb()
{
}

Bomb::~Bomb()
{
}

void Bomb::Init()
{
}

void Bomb::Update()
{
}

void Bomb::Finailize()
{
}

void Bomb::Draw()
{
}

bool Bomb::Shot(DirectX::XMFLOAT3 angle, DirectX::XMFLOAT3 pos)
{
	//爆弾が生きているなら
	if (data.isAlive) return false;

	//爆弾の情報を追加
	data.pos = ConvertXMFLOAT3toXMVECTOR(angle);
	data.isAlive = true;
	data.bombAngle = ConvertXMFLOAT3toXMVECTOR(angle);
}
