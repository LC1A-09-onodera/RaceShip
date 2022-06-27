#include "StageObject.h"
#include "LoadStage.h"
#include "../Camera/Camera.h"

WallObject StageObjects::walls;
GoalObject StageObjects::goals;
EnemyObject StageObjects::enemys;

void WallObject::LoadPosition()
{
	for (auto itr = LoadStage::wallPosition.begin(); itr != LoadStage::wallPosition.end(); ++itr)
	{
		WallObjectEach each;
		ConstInit(each);
		each.position.m128_f32[0] = itr->x;
		each.position.m128_f32[2] = itr->y;
		each.scale = { 2.0f, 4.0f, 2.0f };
		wallsPos.push_back(each);
		WallObjectEach eachR;
		ConstInit(eachR);
		eachR.position.m128_f32[0] = itr->x;
		eachR.position.m128_f32[2] = itr->y;
		eachR.scale = { 2.0f, 4.0f, 2.0f };
		rWallsPos.push_back(eachR);
	}
}

void StageObjects::LoadFile()
{
	LoadStage::wallPosition.clear();
	walls.wallsPos.clear();
	walls.rWallsPos.clear();
	LoadStage::goalPosition.clear();
	goals.goalsPos.clear();
	goals.rGoalsPos.clear();
	LoadStage::LoadStages("test.txt");
	walls.LoadPosition();
	goals.LoadPosition();
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
		for (auto itr = goals.rGoalsPos.begin(); itr != goals.rGoalsPos.end(); ++itr)
		{
			goals.goalModel.Update(&(*itr), isRCamera);
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
		for (auto itr = goals.goalsPos.begin(); itr != goals.goalsPos.end(); ++itr)
		{
			goals.goalModel.Update(&(*itr), isRCamera);
			Draw3DObject(goals.goalModel);
		}
	}
}

void WallModel::Update(WallObjectEach* each, bool rCamera)
{
	if (each != nullptr)
	{
		this->each = *each;
		CalcMatrix(this, each);

		SendVertex();

		ConstBufferDataB0* constMap0 = nullptr;
		if (SUCCEEDED(this->each.constBuff0->Map(0, nullptr, (void**)&constMap0)))
		{
			//constMap0->mat = matWorld * Camera::matView * BaseDirectX::matProjection;
			if (!rCamera)
			{
				constMap0->viewproj = Cameras::camera.matView * BaseDirectX::matProjection;
				constMap0->world = matWorld;
				constMap0->cameraPos = Cameras::camera.eye;
			}
			else
			{
				constMap0->viewproj = Cameras::rCamera.matView * BaseDirectX::matProjection;
				constMap0->world = matWorld;
				constMap0->cameraPos = Cameras::rCamera.eye;
			}

			this->each.constBuff0->Unmap(0, nullptr);
		}

		ConstBufferDataB1* constMap1 = nullptr;
		BaseDirectX::result = this->each.constBuff1->Map(0, nullptr, (void**)&constMap1);
		constMap1->ambient = material.ambient;
		constMap1->diffuse = material.diffuse;
		constMap1->specular = material.specular;
		constMap1->alpha = material.alpha;
		this->each.constBuff1->Unmap(0, nullptr);
	}
	else
	{
		CalcMatrix(this, each);

		SendVertex();

		ConstBufferDataB0* constMap0 = nullptr;
		if (SUCCEEDED(this->each.constBuff0->Map(0, nullptr, (void**)&constMap0)))
		{
			//constMap0->mat = matWorld * Camera::matView * BaseDirectX::matProjection;
			if (!rCamera)
			{
				constMap0->viewproj = Cameras::camera.matView * BaseDirectX::matProjection;
				constMap0->world = matWorld;
				constMap0->cameraPos = Cameras::camera.eye;
			}
			else
			{
				constMap0->viewproj = Cameras::rCamera.matView * BaseDirectX::matProjection;
				constMap0->world = matWorld;
				constMap0->cameraPos = Cameras::rCamera.eye;
			}
			this->each.constBuff0->Unmap(0, nullptr);
		}

		ConstBufferDataB1* constMap1 = nullptr;
		BaseDirectX::result = this->each.constBuff1->Map(0, nullptr, (void**)&constMap1);
		constMap1->ambient = material.ambient;
		constMap1->diffuse = material.diffuse;
		constMap1->specular = material.specular;
		constMap1->alpha = material.alpha;
		this->each.constBuff1->Unmap(0, nullptr);
	}
}

void GoalModel::Update(GoalObjectEach* each, bool rCamera)
{
	if (each != nullptr)
	{
		this->each = *each;
		XMMATRIX matScale, matRot, matTrans;
		const XMFLOAT3& cameraPos = Cameras::camera.eye;
		matScale = XMMatrixScaling(this->each.scale.x, this->each.scale.y, this->each.scale.z);
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(this->each.rotation.z));
		matRot *= XMMatrixRotationX(XMConvertToRadians(this->each.rotation.x));
		matRot *= XMMatrixRotationY(XMConvertToRadians(this->each.rotation.y));
		matTrans = XMMatrixTranslation(this->each.position.m128_f32[0], this->each.position.m128_f32[1], this->each.position.m128_f32[2]);
		matWorld = XMMatrixIdentity();

		//ビルボード
		//if (billboard)
		//{
		//    matWorld *= BaseDirectX::matBillboard;//ビルボードをかける
		//}
		//ビルボードY
		//if (billboard)
		//{
		//    matWorld *= Camera::matBillboardY;//ビルボードをかける
		//}
		matWorld *= matScale;
		matWorld *= matRot;
		matWorld *= matTrans;

		Vertex* vertMap = nullptr;
		BaseDirectX::result = mesh.vertBuff->Map(0, nullptr, (void**)&vertMap);
		if (SUCCEEDED(BaseDirectX::result))
		{
			copy(mesh.vertices.begin(), mesh.vertices.end(), vertMap);
			mesh.vertBuff->Unmap(0, nullptr);    // マップを解除
		}

		ConstBufferDataB0* constMap0 = nullptr;
		if (SUCCEEDED(this->each.constBuff0->Map(0, nullptr, (void**)&constMap0)))
		{
			//constMap0->mat = matWorld * Camera::matView * BaseDirectX::matProjection;
			if (!rCamera)
			{
				constMap0->viewproj = Cameras::camera.matView * BaseDirectX::matProjection;
				constMap0->world = matWorld;
				constMap0->cameraPos = cameraPos;
			}
			else
			{
				constMap0->viewproj = Cameras::rCamera.matView * BaseDirectX::matProjection;
				constMap0->world = matWorld;
				constMap0->cameraPos = Cameras::rCamera.eye;
			}

			this->each.constBuff0->Unmap(0, nullptr);
		}

		ConstBufferDataB1* constMap1 = nullptr;
		BaseDirectX::result = this->each.constBuff1->Map(0, nullptr, (void**)&constMap1);
		constMap1->ambient = material.ambient;
		constMap1->diffuse = material.diffuse;
		constMap1->specular = material.specular;
		constMap1->alpha = material.alpha;
		this->each.constBuff1->Unmap(0, nullptr);
	}
	else
	{
		XMMATRIX matScale, matRot, matTrans;
		const XMFLOAT3& cameraPos = Cameras::camera.eye;
		matScale = XMMatrixScaling(this->each.scale.x, this->each.scale.y, this->each.scale.z);
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(this->each.rotation.z));
		matRot *= XMMatrixRotationX(XMConvertToRadians(this->each.rotation.x));
		matRot *= XMMatrixRotationY(XMConvertToRadians(this->each.rotation.y));
		matTrans = XMMatrixTranslation(this->each.position.m128_f32[0], this->each.position.m128_f32[1], this->each.position.m128_f32[2]);
		matWorld = XMMatrixIdentity();

		//ビルボード
		//if (billboard)
		//{
		//    matWorld *= BaseDirectX::matBillboard;//ビルボードをかける
		//}
		//ビルボードY
		//if (billboard)
		//{
		//    matWorld *= Camera::matBillboardY;//ビルボードをかける
		//}
		matWorld *= matScale;
		matWorld *= matRot;
		matWorld *= matTrans;

		Vertex* vertMap = nullptr;
		BaseDirectX::result = mesh.vertBuff->Map(0, nullptr, (void**)&vertMap);
		if (SUCCEEDED(BaseDirectX::result))
		{
			copy(mesh.vertices.begin(), mesh.vertices.end(), vertMap);
			mesh.vertBuff->Unmap(0, nullptr);    // マップを解除
		}

		ConstBufferDataB0* constMap0 = nullptr;
		if (SUCCEEDED(this->each.constBuff0->Map(0, nullptr, (void**)&constMap0)))
		{
			//constMap0->mat = matWorld * Camera::matView * BaseDirectX::matProjection;
			if (!rCamera)
			{
				constMap0->viewproj = Cameras::camera.matView * BaseDirectX::matProjection;
				constMap0->world = matWorld;
				constMap0->cameraPos = cameraPos;
			}
			else
			{
				constMap0->viewproj = Cameras::rCamera.matView * BaseDirectX::matProjection;
				constMap0->world = matWorld;
				constMap0->cameraPos = Cameras::rCamera.eye;
			}
			this->each.constBuff0->Unmap(0, nullptr);
		}

		ConstBufferDataB1* constMap1 = nullptr;
		BaseDirectX::result = this->each.constBuff1->Map(0, nullptr, (void**)&constMap1);
		constMap1->ambient = material.ambient;
		constMap1->diffuse = material.diffuse;
		constMap1->specular = material.specular;
		constMap1->alpha = material.alpha;
		this->each.constBuff1->Unmap(0, nullptr);
	}
}

void GoalObject::LoadPosition()
{
	for (auto itr = LoadStage::goalPosition.begin(); itr != LoadStage::goalPosition.end(); ++itr)
	{
		GoalObjectEach each;
		ConstInit(each);
		each.position.m128_f32[0] = itr->x;
		each.position.m128_f32[2] = itr->y;
		each.scale = { 2.0f, 4.0f, 2.0f };
		goalsPos.push_back(each);
		GoalObjectEach eachR;
		ConstInit(eachR);
		eachR.position.m128_f32[0] = itr->x;
		eachR.position.m128_f32[2] = itr->y;
		eachR.scale = { 2.0f, 4.0f, 2.0f };
		rGoalsPos.push_back(eachR);
	}
}
