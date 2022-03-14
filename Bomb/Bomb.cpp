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
	//”š’e‚ª¶‚«‚Ä‚¢‚é‚È‚ç
	if (data.isAlive) return false;

	//”š’e‚Ìî•ñ‚ğ’Ç‰Á
	data.pos = ConvertXMFLOAT3toXMVECTOR(angle);
	data.isAlive = true;
	data.bombAngle = ConvertXMFLOAT3toXMVECTOR(angle);
}
