#pragma once
#include <string>
#include <list>
#include <DirectXMath.h>
#include "../../Player/Seling.h"

using namespace DirectX;
using namespace std;


class LoadStage
{
public:
	struct LoadInfo
	{ 
		XMFLOAT3 position;
		XMFLOAT3 rotation;
		XMFLOAT3 scale;
	};
	static list<LoadInfo> wallPosition;
	static list<LoadInfo> enemyPosition;
	static list<LoadInfo> springBoradPosition;
	static LoadInfo goalPosition;
	static LoadInfo playerPos;
	static void LoadStages(string stagePath);
	static void LoadStages(string stagePath, Seling &selingPos);
};