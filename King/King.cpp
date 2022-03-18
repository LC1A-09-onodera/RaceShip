#include "King.h"
#include "../Shader/ShaderManager.h"
#include "../BaseDirectX/Input.h"
void King::LoadModel()
{
	king.CreateModel("King", ShaderManager::playerShader);
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
	king.each.position = {0, 0, 5};
}

void King::Update()
{
	//爆風を受けている時の処理
	if (isWind)
	{

	}
	if (Input::Key(DIK_D))
	{
		king.each.position.m128_f32[0] += 0.5f;
	}
	if (Input::Key(DIK_A))
	{
		king.each.position.m128_f32[0] -= 0.5f;
	}
	if (Input::Key(DIK_W))
	{
		king.each.position.m128_f32[2] += 0.5f;
	}
	if (Input::Key(DIK_S))
	{
		king.each.position.m128_f32[2] -= 0.5f;
	}
}

void King::Draw()
{
	king.Update();
	Draw3DObject(king);
}
