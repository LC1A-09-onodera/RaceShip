#pragma once
#include "../3DModel/Model.h"

class WallObjectEach : public EachInfo
{

};

class WallModel : public Model
{
public:
	void Update(WallObjectEach *each = nullptr, bool rCamera = false);
};

class WallObject
{
public:
	static list<WallObjectEach> wallsPos;
	static WallModel wallModel;
	static void LoadPosition();
};

class GoalObjectEach : public EachInfo
{

};

class GoalModel : public Model
{
public:
	void Update(GoalObjectEach* each = nullptr, bool rCamera = false);
};

class GoalObject
{
public:
	static list<GoalObjectEach> goalsPos;
	static GoalModel goalModel;
	static void LoadPosition();
};

class EnemyObjectEach : public EachInfo
{

};

class EnemyModel : public Model
{
public:
	void Update(EnemyObjectEach* each = nullptr, bool rCamera = false);
};

class EnemyObject
{
public:
	static list<EnemyObjectEach> enemysPos;
	static EnemyModel enemyModel;
	static void LoadPosition();
};

class StageObjects
{
public:
	static WallObject walls;
	static GoalObject goals;
	static EnemyObject enemys;
	static void LoadFile();
	static void Draw();
};