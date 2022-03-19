#include "King.h"
#include "../Shader/ShaderManager.h"
#include "../BaseDirectX/Input.h"
#include "../imgui/ImguiControl.h"

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
	isLife = true;
	LoadModel();
	king.each.position = {0, 0, 5};
}

void King::Update()
{
	//”š•—‚ðŽó‚¯‚Ä‚¢‚éŽž‚Ìˆ—
	if (isWind)
	{
		AddWindForce();
	}
	if (isEnemyHit)
	{
		AddEnemyForce();
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

	StageLineDeth();

	DethUpdate();
}

void King::Draw()
{
	if (!isLife) return;
	king.Update();
	Draw3DObject(king);
}

void King::AddWindForce()
{
	if (!isWind) return;
	king.each.position += ConvertXMFLOAT3toXMVECTOR(windDirection);
	//”š•—‚Ì—Í‚ðŒ¸Š‚³‚¹‚Ä‚¢‚­
	if (windDirection.x > 0.1f)
	{
		windDirection.x -= resistForce;
	}
	else if (windDirection.x < 0.1f)
	{
		windDirection.x += resistForce;
	}
	if (windDirection.z > 0.1f)
	{
		windDirection.z -= resistForce;
	}
	else if (windDirection.z < 0.1f)
	{
		windDirection.z += resistForce;
	}
	//Ý’è’lˆÈ‰º‚È‚ç0‚É
	if (windDirection.x < resistMinForce && windDirection.x > -resistMinForce)
	{
		windDirection.x = 0.0f;
	}
	if (windDirection.z < resistMinForce && windDirection.z > -resistMinForce)
	{
		windDirection.z = 0.0f;
	}
	//
	if (windDirection.x == 0.0f && windDirection.z == 0.0f)
	{
		isWind = false;
	}
}

void King::AddEnemyForce()
{
	if (!isEnemyHit) return;
	king.each.position += ConvertXMFLOAT3toXMVECTOR(enemyDirection);
	//”š•—‚Ì—Í‚ðŒ¸Š‚³‚¹‚Ä‚¢‚­
	if (enemyDirection.x > 0.1f)
	{
		enemyDirection.x -= resistForce;
	}
	else if (enemyDirection.x < 0.1f)
	{
		enemyDirection.x += resistForce;
	}
	if (enemyDirection.z > 0.1f)
	{
		enemyDirection.z -= resistForce;
	}
	else if (enemyDirection.z < 0.1f)
	{
		enemyDirection.z += resistForce;
	}
	//Ý’è’lˆÈ‰º‚È‚ç0‚É
	if (enemyDirection.x < resistMinForce && enemyDirection.x > -resistMinForce)
	{
		enemyDirection.x = 0.0f;
	}
	if (enemyDirection.z < resistMinForce && enemyDirection.z > -resistMinForce)
	{
		enemyDirection.z = 0.0f;
	}
	//
	if (enemyDirection.x == 0.0f && enemyDirection.z == 0.0f)
	{
		isEnemyHit = false;
	}
}

void King::StageLineDeth()
{
	bool result1 = king.each.position.m128_f32[0] > Imgui::dethLine;
	bool result2 = king.each.position.m128_f32[0] < -Imgui::dethLine;
	bool result3 = king.each.position.m128_f32[2] > Imgui::dethLine;
	bool result4 = king.each.position.m128_f32[2] < -Imgui::dethLine;
	if (result1 || result2 || result3 || result4)
	{
		HP--;
	}
}

void King::DethUpdate()
{
	if (HP > 0) return;
	isLife = false;
}
