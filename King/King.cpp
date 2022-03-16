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
	LoadModel();
	king.each.position = {0, 0, 0};
}

void King::Update()
{
	if (Input::Key(DIK_D))
	{
		king.each.position.m128_f32[0] += 0.2f;
	}
	if (Input::Key(DIK_S))
	{
		king.each.position.m128_f32[0] -= 0.2f;
	}
}

void King::Draw()
{
	king.Update();
	Draw3DObject(king);
}
