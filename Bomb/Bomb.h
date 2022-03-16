#pragma once
#include <DirectXMath.h>
#include "../sampleObject/sampleObject.h"
#include "../Enemy/Enemy.h"
struct BombData
{
	DirectX::XMVECTOR pos = {};//爆弾の座標
	DirectX::XMVECTOR bombAngle = { -1, 0, 0 };
	float bombRadius = 0.0f;//爆弾自体の大きさ
	float bombSpeed = 1.0f;//爆弾の速度
	float blastRadius = 10;//爆発の半径
	float blastPower = 10;//爆風の強さ
	int blastTimer = 30;//爆発タイマー(とりあえず使わない)
	bool isAlive = false;//爆弾が現在生きているかどうか
	bool isExplosion = true;//爆風が発生しているかどうか
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

	/// <summary>
	/// 敵との当たり判定
	/// </summary>
	/// <param name="enemyData"></param>
	void EnemyBombCollision(EnemyBase *enemyData);
public:
	bool GetIsAlve() {return data.isAlive;}
	bool GetIsExplosion(){return data.isExplosion;}
private:
	//爆弾が生きている際の更新処理
	void BombUpdate();
	//爆発しているときの更新処理
	void BlastUpdate();

	/// <summary>
	/// 爆弾本体との当たり判定
	/// </summary>
	/// <param name="pos">判定をとる物体の座標</param>
	/// <param name="radius">判定をとる物体の半径</param>
	/// <param name="isHit">当たったか</param>
	bool BombCollision(const XMVECTOR &pos, const float &radius);

	/// <summary>
	/// 爆風との判定
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="radius"></param>
	/// <param name="blastPower"></param>
	/// <returns></returns>
	bool BlastCollision(const XMVECTOR &pos, const float &radius, XMFLOAT3 *blastPower = nullptr);

	/// <summary>
	/// 爆発処理
	/// </summary>
	void Explosion();
private:
	BombData data;
	SampleObject bombObject;
	SampleObject blastObject;
};