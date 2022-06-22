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

class StageObjects
{
public:
	static WallObject walls;

	static void Draw();
};