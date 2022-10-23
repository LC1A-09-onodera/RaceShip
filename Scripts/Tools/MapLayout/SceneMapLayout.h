#pragma once
#include "../../3DModel/Model.h"
#include <list>

class SceneEachInfo
{
public:
	Model model;
	Model rModel;
	list<EachInfo> each;
	list<EachInfo> rEach;
};

class SceneMapLayout
{
	static SceneEachInfo sambasiModel;
	static SceneEachInfo towerModel;
	static SceneEachInfo stoneModel;
public:
	static void LoadModel();
	static void Init();
	static void Update();
	static void WaterFaceDraw();
	static void MainDraw();
};