#include "Player.h"
#include "../BaseDirectX/Input.h"
#include "../Enemy/Enemy.h"
#include "../Camera/Camera.h"
#include "../Sound/Sound.h"

Player::Player()
{
}

Player::~Player()
{
}

Player* Player::GetPlayer()
{
	static Player player;
	return &player;
}

//‰Šú‰»
void Player::Init()
{
	player.shader.vsBlob = LoadShader(L"Resource/HLSL/OBJVertexShader.hlsl", "vs_5_0");
	player.shader.psBlob = LoadShader(L"Resource/HLSL/OBJPixelShader.hlsl", "ps_5_0");
	player.CreateModel("Block");
	player.Init(1);
	water.CreateWater();
}

//XV
void Player::Update()
{
	
}

//•`‰æ
void Player::Draw()
{
	
	Draw3DObject(player);
}

void Player::ReSet()
{
	
}

XMFLOAT3 Player::GetPos()
{
	return pos;
}

void Player::Move()
{
	
}
