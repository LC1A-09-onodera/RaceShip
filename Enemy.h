#pragma once
#include "../3DModel/Model.h"
#include <DirectXMath.h>

using namespace DirectX;

struct Bullet;

enum EnemyAction
{
	ARMATTACK, FOOTATTACK, MOVE, DOWN, STAY
};
//struct Bullet
//{
//	Model bullet;
//	XMFLOAT3 pos;
//	XMFLOAT3 direction;
//	bool isActive;
//	int life;
//};

class Enemy
{
	//モデルデータ
	static const int stayMotionAmount = 5;
	static int stayMotionCount;
	static Model stay[stayMotionAmount];
	static const int armAttackMotionAmount = 5;
	static int armAttackMotionCount;
	static Model armAttack[armAttackMotionAmount];
	static const int footAttackMotionAmount = 5;
	static int footAttackMotionCount;
	static Model footAttack[footAttackMotionAmount];
	//基本的な位置
	static XMFLOAT3 position;
	//各部位のポジション
	static XMFLOAT3 armRPosition;
	static XMFLOAT3 armLPosition;
	static XMFLOAT3 footRPosition;
	static XMFLOAT3 footLPosition;
	static XMFLOAT3 headPosition;
	//向いてる方向
	static XMFLOAT3 direction;
	//行動してるかのフラグ
	static bool isAction ;
	//エネミーの体力
	static int HP;
	//攻撃時に予備エフェクトかけるためのフラグ
	static bool isAttackEffect;
	//行動している時間
	static int actionTime;
	//行動が何番目か
	static int actionNum;
	//瀕死の時に攻撃的になる
	static const int redHP = 20;
	//足攻撃時の時間
	static const int footActionTime = 120;
	//エフェクトを入れるタイミング
	static const int footActionEfectionTime = 30;
	//腕攻撃時の時間
	static const int armActionTime = 120;
	//エフェクトを入れるタイミング
	static const int armActionEffectTime = 30;
	static XMFLOAT3 attackEffectEmitter;
	//動き時間
	static const int moveActionTime = 120;
	//ダウンしているときに時間
	static const int downActionTime = 120;
	//待ち時間
	static const int stayActionTime = 120;
	//弾数
	static const int bulletAmount = 50;
	//玉の時間
	static const int bulletLife = 90;
	//敵の最大HP
	static const int enemyMaxHP = 100;
	//玉
	static Bullet bullet[bulletAmount];
	//出る玉のインデックス
	static int bulletIndex;
	//
	static const int bulletInterval = 5;
	//
	static int bulletTime;
public:
	//初期化
	static void Init();
	//玉の一括初期化
	static void BulletInit();
	//更新
	static void Update();
	//描画
	static void Draw();
	//腕攻撃
	static void ArmAttack();
	//足攻撃
	static void FootAttack();
	//ダウン時に行動不能になるような処理
	static void Down();
	//動くときに使う
	static void Move();
	//待ち時間
	static void Stay();
	//行動を決める
	static void SelectAction();
	//行動後に元に戻す
	static void ReturnNone();
	//玉の動き
	static void BulletUpdate();
	//玉一個発射するためのやーつ
	static void BulletOneInit();
	//玉の方向決める
	static XMFLOAT3 BulletDirection();
	//エフェクトの発生や場所当の変更
	static void EffectControl();
	static void ForPlayerDirection();
	static void ReSet();
};