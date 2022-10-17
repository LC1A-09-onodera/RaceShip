#include "StageObject.h"
#include "LoadStage.h"
#include "../../Camera/Camera.h"
#include "../../Shader/ShaderManager.h"

namespace Editors
{
	WallObject StageObjects::walls;
	GoalObject StageObjects::goals;
	EnemyObject StageObjects::enemys;
	SpringBoradObject StageObjects::springs;

	void WallObject::LoadPosition()
	{
		for (auto itr = LoadStage::wallPosition.begin(); itr != LoadStage::wallPosition.end(); ++itr)
		{
			WallObjectEach each;
			ConstInit(each, BaseDirectX::GetInstance()->dev);
			each.position.m128_f32[0] = itr->position.x;
			each.position.m128_f32[2] = itr->position.y;
			each.rotation.z = itr->rotation.x;
			/*each.rotation.y = itr->rotation.y;
			each.rotation.z = itr->rotation.z;*/
			each.scale = { 1.5f, 2.0f, 0.5f };
			each.m_eachName.clear();
			each.m_eachName = each.m_eachName + "wall";
			wallsPos.push_back(each);
			//EachManager::eahcs.push_back(&(*--wallsPos.end()));
			WallObjectEach eachR;
			ConstInit(eachR, BaseDirectX::GetInstance()->dev);
			eachR.position.m128_f32[0] = itr->position.x;
			eachR.position.m128_f32[2] = itr->position.y;
			eachR.rotation.z = itr->rotation.x;
			/*eachR.rotation.y = itr->rotation.y;
			eachR.rotation.z = itr->rotation.z;*/
			eachR.scale = { 1.5f, 2.0f, 0.5f };
			eachR.m_eachName.clear();
			eachR.m_eachName = eachR.m_eachName + "rWall";
			rWallsPos.push_back(eachR);
			//EachManager::eahcs.push_back(&(*--rWallsPos.end()));
		}
	}

	void StageObjects::LoadFile(Seling& player, const char* path)
	{
		LoadStage::wallPosition.clear();
		walls.wallsPos.clear();
		walls.rWallsPos.clear();
		springs.springPos.clear();
		springs.rSpringPos.clear();
		goals.isActive = false;
		LoadStage::LoadStages(path);
		walls.LoadPosition();
		goals.LoadPosition();
		springs.LoadPosition();
		player.Init();
		player.selingModel.each.position = { LoadStage::playerPos.position.x, 0.0f ,LoadStage::playerPos.position.y ,1.0f };
		player.selingModel.each.rotation.z = LoadStage::playerPos.rotation.x;
	}

	void StageObjects::LoadModel()
	{
		walls.wallModel.CreateModel("Base_Block", ShaderManager::playerShader);
		walls.LoadPosition();
		goals.goalModel.CreateModel("Goal", ShaderManager::playerShader);
		goals.LoadPosition();
		springs.springBoradModel.CreateModel("MapSB", ShaderManager::playerShader);
		springs.LoadPosition();
	}

	void StageObjects::Draw(bool isRCamera)
	{
		if (isRCamera)
		{
			for (auto itr = walls.rWallsPos.begin(); itr != walls.rWallsPos.end(); ++itr)
			{
				
				walls.wallModel.Update(&(*itr), isRCamera);
				Draw3DObject(walls.wallModel);
			}
			for (auto itr = springs.rSpringPos.begin(); itr != springs.rSpringPos.end(); ++itr)
			{
				springs.springBoradModel.Update(&(*itr), isRCamera);
				Draw3DObject(springs.springBoradModel);
			}
			if (goals.isActive)
			{
				goals.goalModel.Update(&(goals.rGoalsPos), isRCamera);
				Draw3DObject(goals.goalModel);
			}
		}
		else
		{
			for (auto itr = walls.wallsPos.begin(); itr != walls.wallsPos.end(); ++itr)
			{
				walls.wallModel.Update(&(*itr), isRCamera);
				Draw3DObject(walls.wallModel);

			}
			for (auto itr = springs.springPos.begin(); itr != springs.springPos.end(); ++itr)
			{
				springs.springBoradModel.Update(&(*itr), isRCamera);
				Draw3DObject(springs.springBoradModel);
			}
			if (goals.isActive)
			{
				goals.goalModel.Update(&(goals.goalsPos), isRCamera);
				Draw3DObject(goals.goalModel);
			}
		}
	}

	void StageObjects::Draw(Camera& f_camera)
	{
		for (auto itr = walls.rWallsPos.begin(); itr != walls.rWallsPos.end(); ++itr)
		{
			walls.wallModel.Update(&(*itr), f_camera);
			Draw3DObject(walls.wallModel);
		}
		for (auto itr = springs.rSpringPos.begin(); itr != springs.rSpringPos.end(); ++itr)
		{
			springs.springBoradModel.Update(&(*itr), f_camera);
			Draw3DObject(springs.springBoradModel);
		}
		goals.goalModel.Update(&(goals.rGoalsPos), f_camera);
		Draw3DObject(goals.goalModel);

	}

	void WallModel::Update(WallObjectEach* f_each, bool rCamera)
	{
		if (f_each != nullptr)
		{
			this->each = *f_each;
		}
		CalcMatrix(this, &this->each);
		SendVertex();
		ConstBufferDataB0* constMap0 = nullptr;
		if (SUCCEEDED(this->each.constBuff0->Map(0, nullptr, (void**)&constMap0)))
		{
			if (!rCamera)
			{
				constMap0->viewproj = Cameras::camera.matView * BaseDirectX::GetInstance()->matProjection;
				constMap0->world = matWorld;
				constMap0->cameraPos = Cameras::camera.eye;
			}
			else
			{
				constMap0->viewproj = Cameras::rCamera.matView * BaseDirectX::GetInstance()->matProjection;
				constMap0->world = matWorld;
				constMap0->cameraPos = Cameras::rCamera.eye;
			}

			this->each.constBuff0->Unmap(0, nullptr);
		}

		ConstBufferDataB1* constMap1 = nullptr;
		BaseDirectX::GetInstance()->result = this->each.constBuff1->Map(0, nullptr, (void**)&constMap1);
		constMap1->ambient = material.ambient;
		constMap1->diffuse = material.diffuse;
		constMap1->specular = material.specular;
		constMap1->alpha = material.alpha;
		this->each.constBuff1->Unmap(0, nullptr);
	}

	void WallModel::Update(WallObjectEach* f_each, Camera& f_camera)
	{
		this->each = *f_each;

		CalcMatrix(this, &this->each);
		SendVertex();
		ConstBufferDataB0* constMap0 = nullptr;
		if (SUCCEEDED(this->each.constBuff0->Map(0, nullptr, (void**)&constMap0)))
		{

			constMap0->viewproj = f_camera.matView * BaseDirectX::GetInstance()->matProjection;
			constMap0->world = matWorld;
			constMap0->cameraPos = f_camera.eye;

			this->each.constBuff0->Unmap(0, nullptr);
		}
		ConstBufferDataB1* constMap1 = nullptr;
		BaseDirectX::GetInstance()->result = this->each.constBuff1->Map(0, nullptr, (void**)&constMap1);
		constMap1->ambient = material.ambient;
		constMap1->diffuse = material.diffuse;
		constMap1->specular = material.specular;
		constMap1->alpha = material.alpha;
		this->each.constBuff1->Unmap(0, nullptr);
	}

	void GoalModel::Update(GoalObjectEach* f_each, bool rCamera)
	{
		if (f_each != nullptr)
		{
			this->each = *f_each;
		}
		CalcMatrix(this, &this->each);

		SendVertex();

		ConstBufferDataB0* constMap0 = nullptr;
		if (SUCCEEDED(this->each.constBuff0->Map(0, nullptr, (void**)&constMap0)))
		{
			if (!rCamera)
			{
				constMap0->viewproj = Cameras::camera.matView * BaseDirectX::GetInstance()->matProjection;
				constMap0->world = matWorld;
				constMap0->cameraPos = Cameras::camera.eye;
			}
			else
			{
				constMap0->viewproj = Cameras::rCamera.matView * BaseDirectX::GetInstance()->matProjection;
				constMap0->world = matWorld;
				constMap0->cameraPos = Cameras::rCamera.eye;
			}

			this->each.constBuff0->Unmap(0, nullptr);
		}

		ConstBufferDataB1* constMap1 = nullptr;
		BaseDirectX::GetInstance()->result = this->each.constBuff1->Map(0, nullptr, (void**)&constMap1);
		constMap1->ambient = material.ambient;
		constMap1->diffuse = material.diffuse;
		constMap1->specular = material.specular;
		constMap1->alpha = material.alpha;
		this->each.constBuff1->Unmap(0, nullptr);

	}

	void GoalModel::Update(GoalObjectEach* f_each, Camera& f_camera)
	{

		this->each = *f_each;
		CalcMatrix(this, &this->each);

		SendVertex();

		ConstBufferDataB0* constMap0 = nullptr;
		if (SUCCEEDED(this->each.constBuff0->Map(0, nullptr, (void**)&constMap0)))
		{
			constMap0->viewproj = f_camera.matView * BaseDirectX::GetInstance()->matProjection;
			constMap0->world = matWorld;
			constMap0->cameraPos = f_camera.eye;
			this->each.constBuff0->Unmap(0, nullptr);
		}

		ConstBufferDataB1* constMap1 = nullptr;
		BaseDirectX::GetInstance()->result = this->each.constBuff1->Map(0, nullptr, (void**)&constMap1);
		constMap1->ambient = material.ambient;
		constMap1->diffuse = material.diffuse;
		constMap1->specular = material.specular;
		constMap1->alpha = material.alpha;
		this->each.constBuff1->Unmap(0, nullptr);
	}

	void GoalObject::LoadPosition()
	{
		GoalObjectEach each;
		ConstInit(each, BaseDirectX::GetInstance()->dev);
		each.position.m128_f32[0] = LoadStage::goalPosition.position.x;
		each.position.m128_f32[2] = LoadStage::goalPosition.position.y;
		each.rotation.z = LoadStage::goalPosition.rotation.x;
		each.scale = { 0.5f, 0.7f, 0.5f };
		each.m_eachName.clear();
		each.m_eachName = each.m_eachName + "goal";
		goalsPos = each;
		GoalObjectEach eachR;
		ConstInit(eachR, BaseDirectX::GetInstance()->dev);
		eachR.position.m128_f32[0] = LoadStage::goalPosition.position.x;
		eachR.position.m128_f32[2] = LoadStage::goalPosition.position.y;
		eachR.rotation.z = LoadStage::goalPosition.rotation.x;
		eachR.scale = { 0.5f, 0.7f, 0.5f };
		eachR.m_eachName.clear();
		eachR.m_eachName = eachR.m_eachName + "rGoal";
		rGoalsPos = eachR;
		isActive = true;
	}

	void SpringBoradModel::Update(SpringBoradEach* f_each, bool rCamera)
	{
		if (f_each != nullptr)
		{
			this->each = *f_each;
		}
		CalcMatrix(this, &this->each);
		SendVertex();
		ConstBufferDataB0* constMap0 = nullptr;
		if (SUCCEEDED(this->each.constBuff0->Map(0, nullptr, (void**)&constMap0)))
		{
			if (!rCamera)
			{
				constMap0->viewproj = Cameras::camera.matView * BaseDirectX::GetInstance()->matProjection;
				constMap0->world = matWorld;
				constMap0->cameraPos = Cameras::camera.eye;
			}
			else
			{
				constMap0->viewproj = Cameras::rCamera.matView * BaseDirectX::GetInstance()->matProjection;
				constMap0->world = matWorld;
				constMap0->cameraPos = Cameras::rCamera.eye;
			}

			this->each.constBuff0->Unmap(0, nullptr);
		}

		ConstBufferDataB1* constMap1 = nullptr;
		BaseDirectX::GetInstance()->result = this->each.constBuff1->Map(0, nullptr, (void**)&constMap1);
		constMap1->ambient = material.ambient;
		constMap1->diffuse = material.diffuse;
		constMap1->specular = material.specular;
		constMap1->alpha = material.alpha;
		this->each.constBuff1->Unmap(0, nullptr);
	}

	void SpringBoradModel::Update(SpringBoradEach* f_each, Camera& f_camera)
	{
		this->each = *f_each;

		CalcMatrix(this, &this->each);
		SendVertex();
		ConstBufferDataB0* constMap0 = nullptr;
		if (SUCCEEDED(this->each.constBuff0->Map(0, nullptr, (void**)&constMap0)))
		{

			constMap0->viewproj = f_camera.matView * BaseDirectX::GetInstance()->matProjection;
			constMap0->world = matWorld;
			constMap0->cameraPos = f_camera.eye;

			this->each.constBuff0->Unmap(0, nullptr);
		}
		ConstBufferDataB1* constMap1 = nullptr;
		BaseDirectX::GetInstance()->result = this->each.constBuff1->Map(0, nullptr, (void**)&constMap1);
		constMap1->ambient = material.ambient;
		constMap1->diffuse = material.diffuse;
		constMap1->specular = material.specular;
		constMap1->alpha = material.alpha;
		this->each.constBuff1->Unmap(0, nullptr);
	}

	void SpringBoradObject::LoadPosition()
	{
		for (auto itr = LoadStage::springBoradPosition.begin(); itr != LoadStage::springBoradPosition.end(); ++itr)
		{
			SpringBoradEach each;
			ConstInit(each, BaseDirectX::GetInstance()->dev);
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
			ConstInit(eachR, BaseDirectX::GetInstance()->dev);
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
}
