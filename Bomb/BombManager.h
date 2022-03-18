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

	bool GetBombAlive();

	void enemyCollision(EnemyBase &data);

	void PlayerCollision(XMFLOAT3 pos, float radius);
private:
	vector<Bomb> bombs;
};