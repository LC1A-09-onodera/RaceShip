#include "Enemy.h"
#include "../Player/Player.h"

Model Enemy::armAttack[Enemy::armAttackMotionAmount];
Model Enemy::footAttack[Enemy::footAttackMotionAmount];
Model Enemy::stay[Enemy::stayMotionAmount];
XMFLOAT3 Enemy::direction;
bool Enemy::isAction;
bool Enemy::isAttackEffect;
int Enemy::HP;
int Enemy::actionNum;
int Enemy::actionTime;
int Enemy::armAttackMotionCount = 0;
int Enemy::footAttackMotionCount = 0;
int Enemy::stayMotionCount = 0;
Bullet Enemy::bullet[Enemy::bulletAmount];
int Enemy::bulletIndex = 0;
const float bulletSpeed = 1.0f;
XMFLOAT3 Enemy::attackEffectEmitter;
const XMFLOAT3 startPosition = { 0, 15, 0 };
XMFLOAT3 Enemy::position = startPosition;
XMFLOAT3 Enemy::armRPosition;
XMFLOAT3 Enemy::armLPosition;
XMFLOAT3 Enemy::footRPosition;
XMFLOAT3 Enemy::footLPosition;
XMFLOAT3 Enemy::headPosition;
int Enemy::bulletTime = 0;

void Enemy::Init()
{
	direction = { 0, -1.0f, 0 };
	HP = enemyMaxHP;
	isAction = false;
	actionNum = NONE;
	actionTime = 0;
	bulletIndex = 0;
	armAttackMotionCount = 0;
	footAttackMotionCount = 0;
	stayMotionCount = 0;
	position = { startPosition };
	for (int i = 0; i < armAttackMotionAmount; i++)
	{
		armAttack[i].CreateModel("untitled");
		armAttack[i].Init(1);
		armAttack[i].position.m128_f32[0] = position.x;
		armAttack[i].position.m128_f32[1] = position.y;
		armAttack[i].position.m128_f32[2] = position.z;
		armAttack[i].scale = { 3, 3, 3 };
	}
	for (int i = 0; i < stayMotionAmount; i++)
	{
		stay[i].CreateModel("bule");
		stay[i].Init(1);
		stay[i].position.m128_f32[0] = position.x;
		stay[i].position.m128_f32[1] = position.y;
		stay[i].position.m128_f32[2] = position.z;
		stay[i].scale = { 3, 3, 3 };
	}
	for (int i = 0; i < bulletAmount; i++)
	{
		bullet[i].bullet.CreateModel("bullet");
		bullet[i].bullet.Init(1);
		bullet[i].isActive = false;
	}
	BulletInit();
}

void Enemy::BulletInit()
{
	for (int i = 0; i < bulletAmount; i++)
	{
		bullet[i].isActive = false;
		bullet[i].life = bulletLife;
		bullet[i].direction = { 0, 1.0f, 0 };
	}
}

void Enemy::Update()
{
	if (isAction)
	{
		switch (actionNum)
		{
		case ARMATTACK:
			ArmAttack();
			break;
		case FOOTATTACK:
			FootAttack();
			break;
		case MOVE:
			Move();
			break;
		case DOWN:
			Down();
			break;
		case STAY:
			Stay();
			break;
		}
	}
	else
	{
		SelectAction();
	}
	if (isAttackEffect)
	{
		EffectControl();
	}
	position.x += 0.1f;
	BulletUpdate();
}

void Enemy::Draw()
{
	switch (actionNum)
	{
	case ARMATTACK:
		for (int i = 0; i < armAttackMotionAmount; i++)
		{
			armAttack[i].position.m128_f32[0] = position.x;
			armAttack[i].position.m128_f32[1] = position.y;
			armAttack[i].position.m128_f32[2] = position.z;
			armAttack[i].Update();
		}
		for (int i = 0; i < bulletAmount; i++)
		{
			if (bullet[i].isActive)
			{
				bullet[i].bullet.position.m128_f32[0] = bullet[i].pos.x;
				bullet[i].bullet.position.m128_f32[1] = bullet[i].pos.y;
				bullet[i].bullet.position.m128_f32[2] = bullet[i].pos.z;
				bullet[i].bullet.Update();
				Draw3DObject(bullet[i].bullet);
			}
		}
		Draw3DObject(armAttack[armAttackMotionCount]);

		break;
	case FOOTATTACK:
		//footAttack[footAttackMotionCount];
		break;
	case MOVE:

		break;
	case DOWN:

		break;
	case STAY:
		for (int i = 0; i < stayMotionAmount; i++)
		{
			stay[i].position.m128_f32[0] = position.x;
			stay[i].position.m128_f32[1] = position.y;
			stay[i].position.m128_f32[2] = position.z;
			stay[i].Update();
		}
		Draw3DObject(stay[stayMotionCount]);
		break;
	}
}

void Enemy::FootAttack()
{
	actionTime++;

	if (actionTime % 25 == 24)
	{
		footAttackMotionCount += 1;
		if (footAttackMotionCount >= footAttackMotionAmount)
		{
			footAttackMotionCount = footAttackMotionAmount - 1;
		}
	}

	//エフェクトを出す
	if (actionTime == footActionEfectionTime)
	{
		isAttackEffect = true;
	}
	//アクションを終わらせる
	if (actionTime >= footActionTime)
	{
		actionTime = 0;
		ReturnNone();
	}
}

void Enemy::Down()
{
	actionTime++;

	//アクションを終わらせる
	if (actionTime >= downActionTime)
	{
		actionTime = 0;
		ReturnNone();
	}
}

void Enemy::Move()
{
	actionTime++;

	//アクションを終わらせる
	if (actionTime >= moveActionTime)
	{
		actionTime = 0;
		ReturnNone();
	}
}

void Enemy::Stay()
{
	actionTime++;

	//アクションを終わらせる
	if (actionTime >= stayActionTime)
	{
		actionTime = 0;
		ReturnNone();
	}
}

void Enemy::SelectAction()
{
	if (actionNum == NONE)
	{
		isAction = true;
		//HPが既定値以上なら通常モード
		if (HP > redHP)
		{
			if (rand() % 2 == 1)
			{
				actionNum = ARMATTACK;
			}
			else
			{
				actionNum = STAY;
			}
		}
		//既定値以下なら攻撃型にシフト
		else
		{

		}
	}
	else
	{
		assert(0);
	}
}

void Enemy::ReturnNone()
{
	isAction = false;
	actionTime = 0;
	actionNum = NONE;
}

void Enemy::BulletUpdate()
{
	for (int i = 0; i < bulletAmount; i++)
	{
		if (bullet[i].isActive)
		{
			bullet[i].pos.x += bullet[i].direction.x * bulletSpeed;
			bullet[i].pos.y += bullet[i].direction.y * bulletSpeed;
			bullet[i].pos.z += bullet[i].direction.z * bulletSpeed;
			bullet[i].life -= 1;
			bullet[i].bullet.position.m128_f32[0] = bullet[i].pos.x;
			bullet[i].bullet.position.m128_f32[1] = bullet[i].pos.y;
			bullet[i].bullet.position.m128_f32[2] = bullet[i].pos.z;
			bullet[i].bullet.Update();

			if (bullet[i].life <= 0)
			{
				bullet[i].isActive = false;
			}
		}
	}
}

void Enemy::BulletOneInit()
{
	//玉の方向を決める
	bullet[bulletIndex].direction = BulletDirection();
	bullet[bulletIndex].isActive = true;
	bullet[bulletIndex].pos = Enemy::position;
	//玉のインデックスを変える
	bulletIndex += 1;
	if (bulletIndex >= bulletAmount)
	{
		bulletIndex = 0;
	}
}

XMFLOAT3 Enemy::BulletDirection()
{
	XMFLOAT3 dire;
	dire.x = Player::GetPosition().x - Enemy::position.x;
	dire.y = Player::GetPosition().y - Enemy::position.y;
	dire.z = Player::GetPosition().z - Enemy::position.z;
	dire = VecNormaliz(dire);
	return dire;
}

void Enemy::ArmAttack()
{
	actionTime++;

	if (actionTime % 25 == 24)
	{
		armAttackMotionCount += 1;
		if (armAttackMotionCount >= armAttackMotionAmount)
		{
			armAttackMotionCount = armAttackMotionAmount - 1;
		}
	}


	//エフェクトを出す
	if (actionTime == armActionEffectTime)
	{
		isAttackEffect = true;
	}
	else if (actionTime > armActionEffectTime)
	{
		bulletTime += 1;
		if (bulletTime % (bulletInterval + 1) == bulletInterval)
		{
			BulletOneInit();
		}
	}
	//アクションを終わらせる
	if (actionTime >= armActionTime)
	{
		armAttackMotionCount = 0;
		actionTime = 0;
		bulletTime = 0;
		ReturnNone();
	}
}

void Enemy::EffectControl()
{
	switch (actionNum)
	{
	case ARMATTACK:
		//ParticleControl::particleMan->CiycleParticle();
		break;
	case FOOTATTACK:

		break;
	}
}

void Enemy::ForPlayerDirection()
{

}

void Enemy::ReSet()
{
	direction = { 0, -1.0f, 0 };
	HP = enemyMaxHP;
	isAction = false;
	actionNum = NONE;
	actionTime = 0;
	bulletIndex = 0;
	armAttackMotionCount = 0;
	footAttackMotionCount = 0;
	stayMotionCount = 0;
	position = startPosition;
	bulletTime = 0;
}
