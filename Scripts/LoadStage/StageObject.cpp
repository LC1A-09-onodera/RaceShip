#include "StageObject.h"
#include "LoadStage.h"
#include "../Camera/Camera.h"
#include "../Shader/ShaderManager.h"

WallObject StageObjects::walls;
GoalObject StageObjects::goals;
EnemyObject StageObjects::enemys;
SpringBoradObject StageObjects::springs;

void WallObject::LoadPosition(BaseDirectX& baseDirectX)
{
	for (auto itr = LoadStage::wallPosition.begin(); itr != LoadStage::wallPosition.end(); ++itr)
	{
		WallObjectEach each;
		ConstInit(each, baseDirectX.dev);
		each.position.m128_f32[0] = itr->position.x;
		each.position.m128_f32[2] = itr->position.y;
		each.rotation.z = itr->rotation.x;
		/*each.rotation.y = itr->rotation.y;
		each.rotation.z = itr->rotation.z;*/
		each.position.m128_f32[2] = itr->position.y;
		each.scale = { 2.0f, 4.0f, 2.0f };
		each.m_eachName.clear();
		each.m_eachName = each.m_eachName + "wall";
		wallsPos.push_back(each);
		//EachManager::eahcs.push_back(&(*--wallsPos.end()));
		WallObjectEach eachR;
		ConstInit(eachR, baseDirectX.dev);
		eachR.position.m128_f32[0] = itr->position.x;
		eachR.position.m128_f32[2] = itr->position.y;
		eachR.rotation.z = itr->rotation.x;
		/*eachR.rotation.y = itr->rotation.y;
		eachR.rotation.z = itr->rotation.z;*/
		eachR.scale = { 2.0f, 4.0f, 2.0f };
		eachR.m_eachName.clear();
		eachR.m_eachName = eachR.m_eachName + "rWall";
		rWallsPos.push_back(eachR);
		//EachManager::eahcs.push_back(&(*--rWallsPos.end()));
	}
}

void StageObjects::LoadFile(BaseDirectX& baseDirectX, Seling& player, const char* path)
{
	LoadStage::wallPosition.clear();
	walls.wallsPos.clear();
	walls.rWallsPos.clear();
	springs.springPos.clear();
	springs.rSpringPos.clear();
	goals.isActive = false;
	LoadStage::LoadStages(path);
	walls.LoadPosition(baseDirectX);
	goals.LoadPosition(baseDirectX);
	springs.LoadPosition(baseDirectX);
	player.Init(baseDirectX);
	player.selingModel.each.position = { LoadStage::playerPos.position.x, 0.0f ,LoadStage::playerPos.position.y ,1.0f };
	player.selingModel.each.rotation.z = LoadStage::playerPos.rotation.x;
}

void StageObjects::LoadModel(BaseDirectX& baseDirectX)
{
	walls.wallModel.CreateModel(baseDirectX, "MapWall", ShaderManager::playerShader);
	walls.LoadPosition(baseDirectX);
	goals.goalModel.CreateModel(baseDirectX, "Goal", ShaderManager::playerShader);
	goals.LoadPosition(baseDirectX);
	springs.springBoradModel.CreateModel(baseDirectX, "MapSB", ShaderManager::playerShader);
	springs.LoadPosition(baseDirectX);
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
		/*for (auto itr = springs.rSpringPos.begin(); itr != springs.rSpringPos.end(); ++itr)
		{
			springs.springBoradModel.Update(baseDirectX, &(*itr), isRCamera);
			Draw3DObject(baseDirectX, springs.springBoradModel);
		}*/
		if (goals.isActive)
		{
			goals.goalModel.Update(baseDirectX, &(goals.rGoalsPos), isRCamera);
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
		/*for (auto itr = springs.springPos.begin(); itr != springs.springPos.end(); ++itr)
		{
			springs.springBoradModel.Update(baseDirectX, &(*itr), isRCamera);
			Draw3DObject(baseDirectX, springs.springBoradModel);
		}*/
		if (goals.isActive)
		{
			goals.goalModel.Update(baseDirectX, &(goals.goalsPos), isRCamera);
			Draw3DObject(baseDirectX, goals.goalModel);
		}
	}
}

void StageObjects::Draw(BaseDirectX& baseDirectX, Camera& f_camera)
{
	for (auto itr = walls.rWallsPos.begin(); itr != walls.rWallsPos.end(); ++itr)
	{
		walls.wallModel.Update(baseDirectX, &(*itr), f_camera);
		Draw3DObject(baseDirectX, walls.wallModel);
	}
	/*for (auto itr = springs.rSpringPos.begin(); itr != springs.rSpringPos.end(); ++itr)
	{
		springs.springBoradModel.Update(baseDirectX, &(*itr), f_camera);
		Draw3DObject(baseDirectX, walls.wallModel);
	}*/
	goals.goalModel.Update(baseDirectX, &(goals.rGoalsPos), f_camera);
	Draw3DObject(baseDirectX, goals.goalModel);

}

void WallModel::Update(BaseDirectX& baseDirectX, WallObjectEach* f_each, bool rCamera)
{
	if (f_each != nullptr)
	{
		this->each = *f_each;
	}
	CalcMatrix(this, &this->each);
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

void WallModel::Update(BaseDirectX& baseDirectX, WallObjectEach* f_each, Camera& f_camera)
{
	this->each = *f_each;

	CalcMatrix(this, &this->each);
	SendVertex(baseDirectX);
	ConstBufferDataB0* constMap0 = nullptr;
	if (SUCCEEDED(this->each.constBuff0->Map(0, nullptr, (void**)&constMap0)))
	{

		constMap0->viewproj = f_camera.matView * baseDirectX.matProjection;
		constMap0->world = matWorld;
		constMap0->cameraPos = f_camera.eye;

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

void GoalModel::Update(BaseDirectX& baseDirectX, GoalObjectEach* f_each, bool rCamera)
{
	if (f_each != nullptr)
	{
		this->each = *f_each;
	}
	CalcMatrix(this, &this->each);

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

void GoalModel::Update(BaseDirectX& baseDirectX, GoalObjectEach* f_each, Camera& f_camera)
{

	this->each = *f_each;
	CalcMatrix(this, &this->each);

	SendVertex(baseDirectX);

	ConstBufferDataB0* constMap0 = nullptr;
	if (SUCCEEDED(this->each.constBuff0->Map(0, nullptr, (void**)&constMap0)))
	{
		constMap0->viewproj = f_camera.matView * baseDirectX.matProjection;
		constMap0->world = matWorld;
		constMap0->cameraPos = f_camera.eye;
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
	GoalObjectEach each;
	ConstInit(each, baseDirectX.dev);
	each.position.m128_f32[0] = LoadStage::goalPosition.position.x;
	each.position.m128_f32[2] = LoadStage::goalPosition.position.y;
	each.rotation.z = LoadStage::goalPosition.rotation.x;
	each.scale = { 0.5f, 0.7f, 0.5f };
	each.m_eachName.clear();
	each.m_eachName = each.m_eachName + "goal";
	goalsPos = each;
	GoalObjectEach eachR;
	ConstInit(eachR, baseDirectX.dev);
	eachR.position.m128_f32[0] = LoadStage::goalPosition.position.x;
	eachR.position.m128_f32[2] = LoadStage::goalPosition.position.y;
	eachR.rotation.z = LoadStage::goalPosition.rotation.x;
	eachR.scale = { 0.5f, 0.7f, 0.5f };
	eachR.m_eachName.clear();
	eachR.m_eachName = eachR.m_eachName + "rGoal";
	rGoalsPos = eachR;
	isActive = true;
}

void SpringBoradModel::Update(BaseDirectX& baseDirectX, SpringBoradEach* f_each, bool rCamera)
{
	if (f_each != nullptr)
	{
		this->each = *f_each;
	}
	CalcMatrix(this, &this->each);
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

void SpringBoradModel::Update(BaseDirectX& baseDirectX, SpringBoradEach* f_each, Camera& f_camera)
{
	this->each = *f_each;

	CalcMatrix(this, &this->each);
	SendVertex(baseDirectX);
	ConstBufferDataB0* constMap0 = nullptr;
	if (SUCCEEDED(this->each.constBuff0->Map(0, nullptr, (void**)&constMap0)))
	{

		constMap0->viewproj = f_camera.matView * baseDirectX.matProjection;
		constMap0->world = matWorld;
		constMap0->cameraPos = f_camera.eye;

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

void SpringBoradObject::LoadPosition(BaseDirectX& baseDirectX)
{
	for (auto itr = LoadStage::springBoradPosition.begin(); itr != LoadStage::springBoradPosition.end(); ++itr)
	{
		SpringBoradEach each;
		ConstInit(each, baseDirectX.dev);
		each.position.m128_f32[0] = itr->position.x;
		each.position.m128_f32[2] = itr->position.y;
		each.rotation.z = itr->rotation.x;
		/*each.rotation.y = itr->rotation.y;
		each.rotation.z = itr->rotation.z;*/
		each.position.m128_f32[2] = itr->position.y;
		each.scale = { 2.0f, 4.0f, 2.0f };
		each.m_eachName.clear();
		each.m_eachName = each.m_eachName + "SpringBorad";
		springPos.push_back(each);
		SpringBoradEach eachR;
		ConstInit(eachR, baseDirectX.dev);
		eachR.position.m128_f32[0] = itr->position.x;
		eachR.position.m128_f32[2] = itr->position.y;
		eachR.rotation.z = itr->rotation.x;
		/*eachR.rotation.y = itr->rotation.y;
		eachR.rotation.z = itr->rotation.z;*/
		eachR.scale = { 2.0f, 4.0f, 2.0f };
		eachR.m_eachName.clear();
		eachR.m_eachName = eachR.m_eachName + "SpringBorad";
		rSpringPos.push_back(eachR);
	}
}
