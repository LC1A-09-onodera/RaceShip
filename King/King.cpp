#include "King.h"
#include "../Shader/ShaderManager.h"
#include "../BaseDirectX/Input.h"
void King::LoadModel()
{
	king.CreateModel("maru", ShaderManager::playerShader);
}

void King::SetPosition(XMFLOAT3 &position)
{
	king.each.position = ConvertXMFLOAT3toXMVECTOR(position);
}

void King::Init(int HP)
{
	this->HP = HP;
	isWind = false;
	LoadModel();
	king.each.position = {0, 0, 0};
}

void King::Update()
{
	//”š•—‚ğó‚¯‚Ä‚¢‚é‚Ìˆ—
	if (isWind)
	{

	}
}

void King::Draw()
{
	king.Update();
	Draw3DObject(king);
}
