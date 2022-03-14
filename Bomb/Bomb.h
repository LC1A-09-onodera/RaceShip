#pragma once
#include <DirectXMath.h>

struct BombData
{
	DirectX::XMVECTOR pos;
	DirectX::XMVECTOR bombRadius;
	DirectX::XMVECTOR blastRadius;
	float blastPower;
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
private:

};