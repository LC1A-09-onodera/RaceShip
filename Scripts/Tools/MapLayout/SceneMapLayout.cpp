#include "SceneMapLayout.h"
#include "../../Shader/ShaderManager.h"
#include "../../BaseDirectX/BaseDirectX.h"

SceneEachInfo SceneMapLayout::sambasiModel;
SceneEachInfo SceneMapLayout::towerModel;
SceneEachInfo SceneMapLayout::stoneModel;

void SceneMapLayout::LoadModel()
{
	sambasiModel.model.CreateModel("sambasi", ShaderManager::playerShader, true);
	towerModel.model.CreateModel("tower", ShaderManager::playerShader, true);
	stoneModel.model.CreateModel("stone", ShaderManager::playerShader, true);
}

void SceneMapLayout::Init()
{
	ifstream file;
	file.open("Resource/TextData/Stage/StageLayout.txt");
	if (file.fail())
	{
		assert(0);
	}
	string line;

	while (getline(file, line))
	{
		istringstream line_stream(line);

		string key;
		getline(line_stream, key, ' ');

		EachInfo info;
		ConstInit(info, BaseDirectX::GetInstance()->dev);
		line_stream >> info.position.m128_f32[0];
		line_stream >> info.position.m128_f32[1];
		line_stream >> info.position.m128_f32[2];
		line_stream >> info.rotation.x;
		line_stream >> info.rotation.y;
		line_stream >> info.rotation.z;
		line_stream >> info.scale.x;
		line_stream >> info.scale.y;
		line_stream >> info.scale.z;
		EachInfo rinfo;
		ConstInit(rinfo, BaseDirectX::GetInstance()->dev);
		rinfo.position = info.position;
		rinfo.rotation = info.rotation;
		rinfo.scale = info.scale;
		if (key == "sambasi")
		{
			sambasiModel.each.push_back(info);
			sambasiModel.rEach.push_back(rinfo);
		}
		else if (key == "tower")
		{
			towerModel.each.push_back(info);
			towerModel.rEach.push_back(rinfo);
		}
		else if (key == "stone")
		{
			stoneModel.each.push_back(info);
			stoneModel.rEach.push_back(rinfo);
		}
	}
}

void SceneMapLayout::WaterFaceDraw()
{
	for (auto sabmasiRItr = sambasiModel.rEach.begin(); sabmasiRItr != sambasiModel.rEach.end(); ++sabmasiRItr)
	{
		sambasiModel.model.Update(&(*sabmasiRItr), true);
		Draw3DObject(sambasiModel.model);
	}
	for (auto towerRItr = towerModel.rEach.begin(); towerRItr != towerModel.rEach.end(); ++towerRItr)
	{
		towerModel.model.Update(&(*towerRItr), true);
		Draw3DObject(towerModel.model);
	}
	for (auto stoneItr = stoneModel.rEach.begin(); stoneItr != stoneModel.rEach.end(); ++stoneItr)
	{
		stoneModel.model.Update(&(*stoneItr), true);
		Draw3DObject(stoneModel.model);
	}
}

void SceneMapLayout::MainDraw()
{
	for (auto sabmasiItr = sambasiModel.each.begin(); sabmasiItr != sambasiModel.each.end(); ++sabmasiItr)
	{
		sambasiModel.model.Update(&(*sabmasiItr), false);
		Draw3DObject(sambasiModel.model);
	}
	for (auto towerItr = towerModel.each.begin(); towerItr != towerModel.each.end(); ++towerItr)
	{
		towerModel.model.Update(&(*towerItr), false);
		Draw3DObject(towerModel.model);
	}
	for (auto stoneItr = stoneModel.each.begin(); stoneItr != stoneModel.each.end(); ++stoneItr)
	{
		stoneModel.model.Update(&(*stoneItr), false);
		Draw3DObject(stoneModel.model);
	}
}
