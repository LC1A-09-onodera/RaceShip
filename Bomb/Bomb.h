#pragma once
#include <DirectXMath.h>


struct BombData
{
	DirectX::XMVECTOR pos = {};//爆弾の座標
	DirectX::XMVECTOR bombAngle = {0, 0, 0};
	float bombRadius = 0;//爆弾自体の大きさ
	float blastRadius = 10;//爆発の半径
	float blastPower = 10;//爆風の強さ
	int blastTimer = 30;//爆発タイマー(とりあえず使わない)
	bool isAlive = false;
};
class Bomb
{
public:
	Bomb();
	~Bomb();
	void Init();
	void Update();
	void Finailize();
	void Draw();

	/// <summary>
	/// 発射
	/// </summary>
	/// <param name="angle">向き</param>
	/// <param name="pos"></param>
	/// <returns></returns>
	bool Shot(DirectX::XMFLOAT3 angle, DirectX::XMFLOAT3 pos);
private:
	BombData data;
};