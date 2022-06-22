#pragma once
#include <string>
#include <list>
#include <DirectXMath.h>

using namespace DirectX;
using namespace std;


class LoadStage
{
public:
	static list<XMFLOAT3> wallPosition;
	static list<XMFLOAT3> goalPosition;
	static list<XMFLOAT3> enemyPosition;
	static void LoadStages(string stagePath);
};