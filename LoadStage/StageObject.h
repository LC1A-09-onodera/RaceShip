#pragma once
#include "../3DModel/Model.h"

class WallObjectEach : public EachInfo
{

};

class WallModel : public Model
{
public:
	void Update(BaseDirectX& baseDirectX, WallObjectEach *each = nullptr, bool rCamera = false);
};

class WallObject
{
public:
	list<WallObjectEach> wallsPos;
	list<WallObjectEach> rWallsPos;
	WallModel wallModel;
	void LoadPosition(BaseDirectX &baseDirectX);
};

class GoalObjectEach : public EachInfo
{

};

class GoalModel : public Model
{
public:
	void Update(BaseDirectX& baseDirectX, GoalObjectEach* each = nullptr, bool rCamera = false);
};

class GoalObject
{
public:
	list<GoalObjectEach> goalsPos;
	list<GoalObjectEach> rGoalsPos;
	GoalModel goalModel;
	void LoadPosition(BaseDirectX& baseDirectX);
};

class EnemyObjectEach : public EachInfo
{

};

class EnemyModel : public Model
{
public:
	void Update(BaseDirectX& baseDirectX, EnemyObjectEach* each = nullptr, bool rCamera = false);
};

class EnemyObject
{
public:
	list<EnemyObjectEach> enemysPos;
	list<EnemyObjectEach> rEnemysPos;
	EnemyModel enemyModel;
	void LoadPosition(BaseDirectX& baseDirectX);
};

class StageObjects
{
public:
	static WallObject walls;
	static GoalObject goals;
	static EnemyObject enemys;
	static void LoadFile(BaseDirectX& baseDirectX);
	static void LoadModel(BaseDirectX &baseDirectX);
	static void Draw(BaseDirectX &baseDirectX, bool isRCamera = false);
};