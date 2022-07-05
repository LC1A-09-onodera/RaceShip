#include "StageObject.h"
#include "LoadStage.h"
#include "../Camera/Camera.h"
#include "../Shader/ShaderManager.h"

WallObject StageObjects::walls;
GoalObject StageObjects::goals;
EnemyObject StageObjects::enemys;

void WallObject::LoadPosition(BaseDirectX& baseDirectX)
{
	for (auto itr = LoadStage::wallPosition.begin(); itr != LoadStage::wallPosition.end(); ++itr)
	{
		WallObjectEach each;
		ConstInit(each, baseDirectX.dev);
		each.position.m128_f32[0] = itr->x;
		each.position.m128_f32[2] = itr->y;
		each.scale = { 2.0f, 4.0f, 2.0f };
		wallsPos.push_back(each);
		WallObjectEach eachR;
		ConstInit(eachR, baseDirectX.dev);
		eachR.position.m128_f32[0] = itr->x;
		eachR.position.m128_f32[2] = itr->y;
		eachR.scale = { 2.0f, 4.0f, 2.0f };
		rWallsPos.push_back(eachR);
	}
}

void StageObjects::LoadFile(BaseDirectX& baseDirectX, const char* path)
{
	LoadStage::wallPosition.clear();
	walls.wallsPos.clear();
	walls.rWallsPos.clear();
	LoadStage::goalPosition.clear();
	goals.goalsPos.clear();
	goals.rGoalsPos.clear();
	LoadStage::LoadStages(path);
	walls.LoadPosition(baseDirectX);
	goals.LoadPosition(baseDirectX);
}

void StageObjects::LoadModel(BaseDirectX& baseDirectX)
{
	walls.wallModel.CreateModel(baseDirectX, "MapWall", ShaderManager::playerShader);
	walls.LoadPosition(baseDirectX);
	goals.goalModel.CreateModel(baseDirectX, "Goal", ShaderManager::playerShader);
	goals.LoadPosition(baseDirectX);
}

void StageObjects::Draw(BaseDirectX& baseDirectX, bool isRCamera)
{
	if (isRCamera)
	{
		for (auto itr = walls.rWallsPos.begin(); itr != walls.rWallsPos.end(); ++itr)
		{
			walls.wallModel.Update(baseDirectX, &(*itr), isRCamera);
			Draw3DObject(baseDirectX, walls.wallModel);
		}
		for (auto itr = goals.rGoalsPos.begin(); itr != goals.rGoalsPos.end(); ++itr)
		{
			goals.goalModel.Update(baseDirectX, &(*itr), isRCamera);
			Draw3DObject(baseDirectX, goals.goalModel);
		}
	}
	else
	{
		for (auto itr = walls.wallsPos.begin(); itr != walls.wallsPos.end(); ++itr)
		{
			walls.wallModel.Update(baseDirectX, &(*itr), isRCamera);
			Draw3DObject(baseDirectX, walls.wallModel);
		}
		for (auto itr = goals.goalsPos.begin(); itr != goals.goalsPos.end(); ++itr)
		{
			goals.goalModel.Update(baseDirectX, &(*itr), isRCamera);
			Draw3DObject(baseDirectX, goals.goalModel);
		}
	}
}

void WallModel::Update(BaseDirectX& baseDirectX, WallObjectEach* each, bool rCamera)
{
	if (each != nullptr)
	{
		this->each = *each;
	}
	CalcMatrix(this, each);
	SendVertex(baseDirectX);
	ConstBufferDataB0* constMap0 = nullptr;
	if (SUCCEEDED(this->each.constBuff0->Map(0, nullptr, (void**)&constMap0)))
	{
		if (!rCamera)
		{
			constMap0->viewproj = Cameras::camera.matView * baseDirectX.matProjection;
			constMap0->world = matWorld;
			constMap0->cameraPos = Cameras::camera.eye;
		}
		else
		{
			constMap0->viewproj = Cameras::rCamera.matView * baseDirectX.matProjection;
			constMap0->world = matWorld;
			constMap0->cameraPos = Cameras::rCamera.eye;
		}

		this->each.constBuff0->Unmap(0, nullptr);
	}

	ConstBufferDataB1* constMap1 = nullptr;
	baseDirectX.result = this->each.constBuff1->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = material.ambient;
	constMap1->diffuse = material.diffuse;
	constMap1->specular = material.specular;
	constMap1->alpha = material.alpha;
	this->each.constBuff1->Unmap(0, nullptr);
}

void GoalModel::Update(BaseDirectX& baseDirectX, GoalObjectEach* each, bool rCamera)
{
	if (each != nullptr)
	{
		this->each = *each;
	}
	CalcMatrix(this, each);

	SendVertex(baseDirectX);

	ConstBufferDataB0* constMap0 = nullptr;
	if (SUCCEEDED(this->each.constBuff0->Map(0, nullptr, (void**)&constMap0)))
	{
		if (!rCamera)
		{
			constMap0->viewproj = Cameras::camera.matView * baseDirectX.matProjection;
			constMap0->world = matWorld;
			constMap0->cameraPos = Cameras::camera.eye;
		}
		else
		{
			constMap0->viewproj = Cameras::rCamera.matView * baseDirectX.matProjection;
			constMap0->world = matWorld;
			constMap0->cameraPos = Cameras::rCamera.eye;
		}

		this->each.constBuff0->Unmap(0, nullptr);
	}

	ConstBufferDataB1* constMap1 = nullptr;
	baseDirectX.result = this->each.constBuff1->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = material.ambient;
	constMap1->diffuse = material.diffuse;
	constMap1->specular = material.specular;
	constMap1->alpha = material.alpha;
	this->each.constBuff1->Unmap(0, nullptr);

}

void GoalObject::LoadPosition(BaseDirectX& baseDirectX)
{
	for (auto itr = LoadStage::goalPosition.begin(); itr != LoadStage::goalPosition.end(); ++itr)
	{
		GoalObjectEach each;
		ConstInit(each, baseDirectX.dev);
		each.position.m128_f32[0] = itr->x;
		each.position.m128_f32[2] = itr->y;
		each.scale = { 0.5f, 0.7f, 0.5f };
		goalsPos.push_back(each);
		GoalObjectEach eachR;
		ConstInit(eachR, baseDirectX.dev);
		eachR.position.m128_f32[0] = itr->x;
		eachR.position.m128_f32[2] = itr->y;
		eachR.scale = { 0.5f, 0.7f, 0.5f };
		rGoalsPos.push_back(eachR);
	}
}
