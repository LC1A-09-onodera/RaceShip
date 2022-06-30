#include "../VoiceUDPRecive/VoiceUDPRecive.h"
#include "Seling.h"
#include "../BaseDirectX/Input.h"
#include "../Shader/ShaderManager.h"
#include "../LoadStage/StageObject.h"

void Seling::ForceUpdate()
{
	if (!isMoveForce) return;

	addForce.x += -(addForce.x / subForce);
	addForce.y += -(addForce.y / subForce);
	addForce.z += -(addForce.z / subForce);

	if (Lenght(addForce) < 0.05f)
	{
		isMoveForce = false;
	}
}


void Seling::LoadModel(BaseDirectX& baseDirectX)
{
	seling.CreateModel(baseDirectX, "Triangle", ShaderManager::playerShader, false);
	Init();
}

void Seling::Init()
{
	frontDirection = { 0, 0 ,1.0f };
	addForce = { 0, 0 ,0 };
}

void Seling::Update()
{
	Move();

	ForceUpdate();

	HitWall();

	HitGoal();

	ForceAttach();
}

void Seling::Draw(BaseDirectX& baseDirectX, bool isRCamera)
{
	seling.Update(baseDirectX, &seling.each, isRCamera);
	Draw3DObject(baseDirectX, seling);
}

void Seling::AddForce(XMFLOAT3& force)
{
	addForce.x = addForce.x + force.x;
	addForce.y = addForce.y + force.y;
	addForce.z = addForce.z + force.z;
	addForce.x = ShlomonMath::Clamp(addForce.x, -maxForce.x, maxForce.x);
	addForce.y = ShlomonMath::Clamp(addForce.y, -maxForce.y, maxForce.y);
	addForce.z = ShlomonMath::Clamp(addForce.z, -maxForce.z, maxForce.z);
	isMoveForce = true;
}

void Seling::ForceAttach()
{
	seling.each.position.m128_f32[0] += addForce.x;
	seling.each.position.m128_f32[1] += addForce.y;
	seling.each.position.m128_f32[2] += addForce.z;
}

void Seling::Move()
{
	if (VoiceReciver::GetRight() || Input::KeyTrigger(DIK_D))
	{
		angle += addSelingAngle;
	}
	if (VoiceReciver::GetLeft() || Input::KeyTrigger(DIK_A))
	{
		angle -= addSelingAngle;
	}

	frontDirection = { ShlomonMath::Sin(angle), 0, ShlomonMath::Cos(angle) };

	if (VoiceReciver::GetFront() || Input::Key(DIK_W))
	{
		AddForce(XMFLOAT3(frontDirection.x * addForcePower, frontDirection.y * addForcePower, frontDirection.z * addForcePower));
	}
	if (VoiceReciver::GetBack() || Input::Key(DIK_S))
	{
		AddForce(XMFLOAT3(frontDirection.x * -addForcePower, frontDirection.y * -addForcePower, frontDirection.z * -addForcePower));
	}

	VoiceReciver::SetRight(false);
	VoiceReciver::SetLeft(false);
	VoiceReciver::SetFront(false);
	VoiceReciver::SetBack(false);

	seling.each.rotation = ShlomonMath::EaseInQuad(seling.each.rotation, XMFLOAT3(seling.each.rotation.x, seling.each.rotation.y, angle), 0.3f);
}

void Seling::HitWall()
{
	for (auto itr = StageObjects::walls.wallsPos.begin(); itr != StageObjects::walls.wallsPos.end(); ++itr)
	{
		if (Lenght(seling.each.position, itr->position) <= 2.0f)
		{
			XMFLOAT3 vec = ConvertXMVECTORtoXMFLOAT3(itr->position - seling.each.position);
			vec = Normalize(vec);
			const float rezist = 7.0f;
			bool blockR = itr->position.m128_f32[0] > seling.each.position.m128_f32[0];
			bool blockFront = itr->position.m128_f32[2] > seling.each.position.m128_f32[2];
			bool hitRight = blockR && addForce.x > 0;
			bool hitLeft = !blockR && addForce.x < 0;
			bool hitFront = blockFront && addForce.z > 0;
			bool hitBack = !blockFront && addForce.z < 0;
			if (hitRight || hitLeft)
			{
				addForce.x = -addForce.x / rezist;
			}
			if (hitFront || hitBack)
			{
				addForce.z = -addForce.z / rezist;
			}
		}
	}
}

void Seling::HitGoal()
{
	for (auto itr = StageObjects::goals.goalsPos.begin(); itr != StageObjects::goals.goalsPos.end(); ++itr)
	{
		if (Lenght(seling.each.position, itr->position) <= 2.0f)
		{
			isGoal = true;
		}
	}
}
