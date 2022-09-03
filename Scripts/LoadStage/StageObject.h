#pragma once
#include "../3DModel/Model.h"
#include "../Player/Seling.h"

class WallObjectEach : public EachInfo
{
	
};

class WallModel : public Model
{
public:
	void Update(BaseDirectX& baseDirectX, WallObjectEach *f_each = nullptr, bool rCamera = false);
	void Update(BaseDirectX& baseDirectX, WallObjectEach* f_each, Camera &f_camera);
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
	void Update(BaseDirectX& baseDirectX, GoalObjectEach* f_each = nullptr, bool rCamera = false);
	void Update(BaseDirectX& baseDirectX, GoalObjectEach* f_each ,Camera &f_camera);
};

class GoalObject
{
public:
	bool isActive = false;
	GoalObjectEach goalsPos;
	GoalObjectEach rGoalsPos;
	GoalModel goalModel;
	void LoadPosition(BaseDirectX& baseDirectX);
};

class EnemyObjectEach : public EachInfo
{

};

class EnemyModel : public Model
{
public:
	void Update(BaseDirectX& baseDirectX, GoalObjectEach* f_each = nullptr, bool rCamera = false);
	void Update(BaseDirectX& baseDirectX, GoalObjectEach* f_each, Camera& f_camera);
};

class EnemyObject
{
public:
	list<EnemyObjectEach> enemysPos;
	list<EnemyObjectEach> rEnemysPos;
	EnemyModel enemyModel;
	void LoadPosition(BaseDirectX& baseDirectX);
};

class SpringBoradEach : public EachInfo
{
	
};

class SpringBoradModel : public Model
{
public:
	void Update(BaseDirectX& baseDirectX, SpringBoradEach* f_each = nullptr, bool rCamera = false);
	void Update(BaseDirectX& baseDirectX, SpringBoradEach* f_each, Camera& f_camera);
};

class SpringBoradObject
{
public:
	list<SpringBoradEach> springPos;
	list<SpringBoradEach> rSpringPos;
	SpringBoradModel springBoradModel;
	void LoadPosition(BaseDirectX& baseDirectX);
};

class StageObjects
{
public:
	static WallObject walls;
	static GoalObject goals;
	static EnemyObject enemys;
	static SpringBoradObject springs;
	static void LoadFile(BaseDirectX& baseDirectX, Seling& player, const char *path);
	static void LoadModel(BaseDirectX &baseDirectX);
	static void Draw(BaseDirectX &baseDirectX, bool isRCamera = false);
	static void Draw(BaseDirectX& baseDirectX, Camera& f_camera);
};