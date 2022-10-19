#pragma once
#include "../../3DModel/Model.h"
#include "../../Player/Seling.h"

namespace Editors
{
	class WallObjectEach : public EachInfo
	{

	};

	class WallModel : public Model
	{
	public:
		void Update(WallObjectEach* f_each = nullptr, bool rCamera = false);
		void Update(WallObjectEach* f_each, Camera& f_camera);
	};

	class WallObject
	{
	public:
		list<WallObjectEach> wallsPos;
		list<WallObjectEach> rWallsPos;
		WallModel wallModel;
		void LoadPosition();
	};

	class GoalObjectEach : public EachInfo
	{

	};

	class GoalModel : public Model
	{
	public:
		void Update(GoalObjectEach* f_each = nullptr, bool rCamera = false);
		void Update(GoalObjectEach* f_each, Camera& f_camera);
	};

	class GoalObject
	{
	public:
		bool isActive = false;
		GoalObjectEach goalsPos;
		GoalObjectEach rGoalsPos;
		GoalModel goalModel;
		void LoadPosition();
	};

	class EnemyObjectEach : public EachInfo
	{

	};

	class EnemyModel : public Model
	{
	public:
		void Update(GoalObjectEach* f_each = nullptr, bool rCamera = false);
		void Update(GoalObjectEach* f_each, Camera& f_camera);
	};

	class EnemyObject
	{
	public:
		list<EnemyObjectEach> enemysPos;
		list<EnemyObjectEach> rEnemysPos;
		EnemyModel enemyModel;
		void LoadPosition();
	};

	class SpringBoradEach : public EachInfo
	{

	};

	class SpringBoradModel : public Model
	{
	public:
		void Update(SpringBoradEach* f_each = nullptr, bool rCamera = false);
		void Update(SpringBoradEach* f_each, Camera& f_camera);
	};

	class SpringBoradObject
	{
	public:
		list<SpringBoradEach> springPos;
		list<SpringBoradEach> rSpringPos;
		SpringBoradModel springBoradModel;
		void LoadPosition();
	};

	class StageObjects
	{
	public:
		static WallObject walls;
		static GoalObject goals;
		static EnemyObject enemys;
		static SpringBoradObject springs;
		static void LoadFile(Seling& player, const char* path);
		static void LoadModel();
		static void Draw(bool isRCamera = false);
		static void Draw(Camera& f_camera);
	};
}
