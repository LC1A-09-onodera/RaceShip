#pragma once
#include <DirectXMath.h>
#include "../sampleObject/sampleObject.h"
#include "../Enemy/Enemy.h"
struct BombData
{
	DirectX::XMVECTOR pos = {};//爆弾の座標
	DirectX::XMVECTOR bombAngle = {0, 0, 0};
	float bombRadius = 0.0f;//爆弾自体の大きさ
	float bombSpeed = 0.0f;//爆弾の速度
	float blastRadius = 10;//爆発の半径
	float blastPower = 10;//爆風の強さ
	int blastTimer = 30;//爆発タイマー(とりあえず使わない)
	bool isAlive = false;//爆弾が現在生きているかどうか
	bool isBlast = false;//爆風が発生しているかどうか
};
class Bomb
{
public:
	Bomb();
	~Bomb();
	//初期化
	void Init();
	//更新処理
	void Update();
	//終了処理
	void Finailize();
	//描画処理
	void Draw();

	/// <summary>
	/// 発射
	/// </summary>
	/// <param name="angle">向き</param>
	/// <param name="pos"></param>
	/// <returns></returns>
	bool Shot(DirectX::XMFLOAT3 angle, DirectX::XMFLOAT3 pos);

	XMFLOAT3 EnemyBombCollision(EnemyBase enemyData);
private:
	//爆弾が生きている際の更新処理
	void BombUpdate();
	//爆発しているときの更新処理
	void BlastUpdate();
private:
	BombData data;
	SampleObject bombObject;
	SampleObject blastObject;
};