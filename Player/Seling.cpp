#include "../VoiceUDPRecive/VoiceUDPRecive.h"
#include "Seling.h"
#include "../BaseDirectX/Input.h"
#include "../Shader/ShaderManager.h"
#include "../LoadStage/StageObject.h"
#include "../imgui/ImguiControl.h"

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
	selingModel.CreateModel(baseDirectX, "boat_v3", ShaderManager::playerShader, false);
	
	Init();
	
	LoadKeys();
}

void Seling::Init()
{
	selingModel.each.position = {0, 0, 0, 1.0f};
	selingModel.each.scale = {1.0f, 1.0f, 0.8f};
	frontDirection = { 0, 0 ,1.0f };
	addForce = { 0, 0 ,0 };
	frontDirection = { 0.0f ,0.0f , 0.0f };
	angle = 0;
	isMoveForce = false;
	addForce = { 0.0f ,0.0f , 0.0f };
	isGoal = false;
}

void Seling::Update()
{
	if (!isGoal)
	{
		Move();

		ForceUpdate();

		HitWall();

		HitGoal();

		ForceAttach();
	}
}

void Seling::Draw(BaseDirectX& baseDirectX, bool isRCamera)
{
	selingModel.Update(baseDirectX, &selingModel.each, isRCamera);
	Draw3DObject(baseDirectX, selingModel);
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
	selingModel.each.position.m128_f32[0] += addForce.x;
	selingModel.each.position.m128_f32[1] += addForce.y;
	selingModel.each.position.m128_f32[2] += addForce.z;
}

void Seling::Move()
{
	if (VoiceReciver::GetRight() || lookToRightKey.GetKey())
	{
		angle += addSelingAngle;
	}
	if (VoiceReciver::GetLeft() || lookToLeftKey.GetKey())
	{
		angle -= addSelingAngle;
	}

	frontDirection = { ShlomonMath::Sin(angle), 0, ShlomonMath::Cos(angle) };

	if (VoiceReciver::GetFront() || goFrontKey.GetKey())
	{
		AddForce(XMFLOAT3(frontDirection.x * addForcePower, frontDirection.y * addForcePower, frontDirection.z * addForcePower));
	}
	if (VoiceReciver::GetBack() || goBackKey.GetKey())
	{
		AddForce(XMFLOAT3(frontDirection.x * -addForcePower, frontDirection.y * -addForcePower, frontDirection.z * -addForcePower));
	}

	VoiceReciver::SetRight(false);
	VoiceReciver::SetLeft(false);
	VoiceReciver::SetFront(false);
	VoiceReciver::SetBack(false);

	selingModel.each.rotation = ShlomonMath::EaseInQuad(selingModel.each.rotation, XMFLOAT3(selingModel.each.rotation.x, selingModel.each.rotation.y, angle), 0.3f);
	Imgui::CameraRotation = -selingModel.each.rotation.z + 270.0f;
}

void Seling::HitWall()
{
	for (auto itr = StageObjects::walls.wallsPos.begin(); itr != StageObjects::walls.wallsPos.end(); ++itr)
	{
		const float WallAndSelingR = 2.9f;
		if (Lenght(selingModel.each.position, itr->position) <= WallAndSelingR)
		{
			XMFLOAT3 vec = ConvertXMVECTORtoXMFLOAT3(itr->position - selingModel.each.position);
			vec = Normalize(vec);
			const float rezist = 7.0f;
			bool blockR = itr->position.m128_f32[0] > selingModel.each.position.m128_f32[0];
			bool blockFront = itr->position.m128_f32[2] > selingModel.each.position.m128_f32[2];
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
		if (Lenght(selingModel.each.position, itr->position) <= 2.0f)
		{
			isGoal = true;
		}
	}
}

void Seling::LoadKeys()
{
	goFrontKey.LoadKey("PlayerGoFront");
	goBackKey.LoadKey("PlayerGoBack");
	lookToRightKey.LoadKey("PlayerLookToRight");
	lookToLeftKey.LoadKey("PlayerLookToLeft");
}
