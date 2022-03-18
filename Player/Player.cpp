#include "Player.h"
#include "../BaseDirectX/Input.h"
#include "../Enemy/Enemy.h"
#include "../Camera/Camera.h"
#include "../Sound/Sound.h"
#include "../Shader/ShaderManager.h"

Player::Player()
{
	XMFLOAT3 pos = { 0,0,0 };
	XMFLOAT3 vec3 = { 0,0,0 };
	int activeCount = 0;
	int invincibleCount = 0;
	bool isActive = true;
	bool isInvincible = false;
	bool isShoot = false;
	bool isDetonating = false;
}

Player::~Player()
{
}

Player* Player::GetPlayer()
{
	static Player player;
	return &player;
}

//初期化
void Player::Init()
{
	player.CreateModel("Block", ShaderManager::playerShader);
	//water.CreateWater();

	XMFLOAT3 pos = { 0,0,0 };		//プレイヤーの座標
	XMFLOAT3 vec3 = { 0,0,0 };		//向いている方向（正規化済）
	int activeCount = 0;			//行動不能カウント
	int invincibleCount = 0;		//無敵カウント
	float bombForce = 0;			//ボムの力保存用
	float enemyForce = 0;			//敵の力保存用
	bool isActive = true;			//行動できるかどうか
	bool isHitBomb = false;			//ボムに当たって飛ばされてるかどうか
	bool isHitEnemy = false;		//敵に当たって飛ばされてるかどうか
	bool isInvincible = false;		//無敵かどうか
	bool isShoot = false;			//射撃中かどうか（弾があるか）
	bool isDetonating = false;		//起爆したかどうか
}

//更新
void Player::Update(bool isBombAlive)
{
	/*移動*/
	CheakIsInput();
	//行動可能かつ吹っ飛んでなければ
	CheakIsActive();

	/*発射トリガー*/
	CheakShootTrigger(isBombAlive);

	/*起爆トリガー*/
	CheakDetonatingTrigger(isBombAlive);

	/*自機が敵に当たった時の判定*/
	CheakHitEnemy();

	/*行動不能とか無敵とか*/
	CalcActiveCount();

	/*ボムの力計算*/
	AddBombForce();

	/*敵の力計算*/
	AddEnemyForce();

	player.each.position = ConvertXMFLOAT3toXMVECTOR(pos);
	player.Update();
}

//描画
void Player::Draw()
{
	Draw3DObject(player);
}

void Player::HitBomb(const float& BombForce)
{
	if (isActive)
	{
		//ボムの力を代入
		bombForce = BombForce;

		//移動制限
		isActive = false;

		//吹っ飛び中にする
		isHitBomb = true;
	}
}

void Player::CheakIsInput()
{
	/*移動*/
	//入力なければリセット
	if (DirectInput::leftStickX() == 0.0f && DirectInput::leftStickY() == 0.0f) { vec3 = { 0,0,0 }; }
	else { vec3.x = DirectInput::leftStickX(); vec3.z = DirectInput::leftStickY(); }
}

void Player::CheakIsActive()
{
	//行動可能かつ吹っ飛んでなければ
	if (isActive && !isHitBomb)
	{
		pos.x += vec3.x * MAX_SPEED;
		pos.z += -vec3.z * MAX_SPEED;
	}
}

void Player::CheakShootTrigger(bool isBombAlive)
{
	/*発射トリガー*/
	if (DirectInput::IsButtonPush(DirectInput::ButtonKind::Button01) && !isBombAlive && isActive) { isShoot = true; }
	else { isShoot = false; }
}

void Player::CheakDetonatingTrigger(bool isBombAlive)
{
	/*起爆トリガー*/
	if (DirectInput::IsButtonPush(DirectInput::ButtonKind::Button01) && isBombAlive && isActive) { isDetonating = true; }
	else { isDetonating = false; }
}

void Player::CheakHitEnemy()
{
	//行動不能時か無敵時か吹っ飛び中は判定無視
	if (!isActive || isInvincible || isHitBomb) { return; }

	bool isHit;
	auto itr = Enemys::enemys.begin();
	for (; itr != Enemys::enemys.end(); ++itr)
	{
		XMFLOAT3 enemyPos = itr->GetPosition();

		//2点間の距離と判定（円）
		isHit = CheakHit(1, 1, pos, enemyPos);

		//当たってなかったらやり直し
		if (!isHit) { continue; }

		//当たったら行動不能にする
		isActive = false;
		break;
	}

	//誰とも当たらなかった場合、終了
	if (isActive) { return; }

	enemyForce = MAX_ENEMY_FORCE;
}

void Player::CalcActiveCount()
{
	/*行動不能とか無敵とか*/
	//無敵かつ行動可能時
	if (isInvincible && isActive)
	{
		if (isInvincible && invincibleCount < MAX_INVICIBLE_COUNT) { invincibleCount++; }
		else { isInvincible = false; }
	}

	//行動不能時
	else if (!isActive)
	{
		if (activeCount < MAX_STAN_COUNT) { activeCount++; }
		else
		{
			activeCount = 0;
			isInvincible = true;
			isActive = true;
		}
	}
}

void Player::AddBombForce()
{
	//吹っ飛び中だけ計算
	if (!isHitBomb) { return; }

	//移動
	pos.x += bombForce * vec3.x;
	pos.z += bombForce * -vec3.z;

	//減算
	bombForce -= RESISTANCE_VALUE;

	//一定量を下回ったら0にして移動再開
	if (bombForce < MINIMUM_FORCE)
	{
		bombForce = 0;
		isActive = true;
		isHitBomb = false;
	}
}

void Player::AddEnemyForce()
{
	//行動不能時だけ計算
	if (isActive) { return; }

	//移動
	pos.x += enemyForce * vec3.x;
	pos.z += enemyForce * -vec3.z;

	//減算
	enemyForce -= RESISTANCE_VALUE;

	//一定量を下回ったら0にする
	if (enemyForce < MINIMUM_FORCE) { enemyForce = 0; }
}
