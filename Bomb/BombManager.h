#pragma once
#include "Bomb.h"
#include <vector>
class BombManager
{
public:
	BombManager();
	~BombManager();
	void Init();
	void Update();
	void Finalize();
	void Draw();
	void Shot(DirectX::XMFLOAT3 angle, DirectX::XMFLOAT3 pos);

	void enemyCollision(EnemyBase &data);
private:
	vector<Bomb> bombs;
};