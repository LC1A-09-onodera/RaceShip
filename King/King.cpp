#include "King.h"

void King::SetPosition(XMFLOAT3 &position)
{
	king.each.position = ConvertXMFLOAT3toXMVECTOR(position);
}

void King::Init(int HP)
{
	this->HP = HP;
}

void King::Update()
{
	king.Update();
}

void King::Draw()
{
	Draw3DObject(king);
}
