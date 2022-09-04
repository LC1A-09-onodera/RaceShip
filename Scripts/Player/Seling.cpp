#include "../VoiceUDPRecive/VoiceUDPRecive.h"
#include "Seling.h"
#include "../BaseDirectX/Input.h"
#include "../Shader/ShaderManager.h"
#include "../LoadStage/StageObject.h"
#include "../imgui/ImguiControl.h"
#include "../Particle/Particle3D.h"
#include "../LoadStage/LoadStage.h"

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

	Init(baseDirectX);

	LoadKeys();

	SpringBoard::LoadModel(baseDirectX);
	/*springBorad.Init(baseDirectX, SpringBoard::Direction::UpToBottom);
	springBorad.m_wid = 10.0f;
	springBorad.m_hi = 2.0f;*/
}

void Seling::Init(BaseDirectX& baseDirectX)
{
	selingModel.each.position = { 0, 0, 0, 1.0f };
	selingModel.each.scale = { 1.0f, 1.0f, 0.8f };
	selingModel.each.m_eachName = "player";
	frontDirection = { 0, 0 ,1.0f };
	addForce = { 0, 0 ,0 };
	frontDirection = { 0.0f ,0.0f , 0.0f };
	angle = 0;
	isMoveForce = false;
	addForce = { 0.0f ,0.0f , 0.0f };
	isGoal = false;
	m_isJump = false;
	m_isLanding = false;
	XMFLOAT3 easeGoal(selingModel.each.rotation.x, selingModel.each.rotation.y, angle);
	selingModel.each.rotation = ShlomonMath::EaseInQuad(selingModel.each.rotation, easeGoal, 1.0f);
	Imgui::CameraRotation = -selingModel.each.rotation.z + 270.0f;
	springBorad.clear();
	for (auto itr = LoadStage::springBoradPosition.begin(); itr != LoadStage::springBoradPosition.end(); ++itr)
	{
		SpringBoard *sp = new SpringBoard();
		sp->m_wid = 2.0f;
		sp->m_vert = 2.0f;
		sp->m_hi = 2.0f;
		sp->m_partEach.position = {itr->position.x, itr->position.y, itr->position.z, 1.0f};
		SpringBoard::Direction dir;
		int rot = static_cast<int>(itr->rotation.x) % 360;
		if (rot == 90)
		{
			dir = SpringBoard::Direction::RightToLeft;
		}
		else if (rot == 180)
		{
			dir = SpringBoard::Direction::UpToBottom;
		}
		else if (rot == 270)
		{
			dir = SpringBoard::Direction::LeftToRight;
		}
		else if (rot == 0)
		{
			dir = SpringBoard::Direction::BottomToUp;
		}
		sp->Init(baseDirectX, dir);
		springBorad.push_back(sp);
	}
}

void Seling::Update()
{
	if (!isGoal)
	{
		Move();

		ForceUpdate();

		HitWall();

		HitGoal();

		SpringBorad();

		ForceAttach();
	}
}
void Seling::Update(bool isPouse)
{
	if (!isGoal && !isPouse)
	{
		Move();

		ForceUpdate();

		HitWall();

		HitGoal();

		SpringBorad();

		ForceAttach();
	}
}

void Seling::Draw(BaseDirectX& baseDirectX, bool isRCamera)
{
	selingModel.Update(baseDirectX, &selingModel.each, isRCamera);
	Draw3DObject(baseDirectX, selingModel);
	for (auto itr = springBorad.begin(); itr != springBorad.end(); ++itr)
	{
		(*itr)->Draw(baseDirectX, isRCamera);
	}
	//springBorad.Draw(baseDirectX, isRCamera);
}

void Seling::Draw(BaseDirectX& baseDirectX, Camera& f_camera)
{
	selingModel.Update(baseDirectX, &selingModel.each, f_camera);
	Draw3DObject(baseDirectX, selingModel);
	for (auto itr = springBorad.begin(); itr != springBorad.end(); ++itr)
	{
		(*itr)->Draw(baseDirectX, f_camera);
	}
	//springBorad.Draw(baseDirectX, f_camera);
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
	if (selingModel.each.position.m128_f32[1] > 0 && !m_isJump)
	{
		selingModel.each.position.m128_f32[1] -= 0.1f;
		if (selingModel.each.position.m128_f32[1] <= 0)
		{
			XMFLOAT3 emitterPos = ConvertXMVECTORtoXMFLOAT3(selingModel.each.position);
			ParticleControl::sheetOfSpray2->Landing(emitterPos);
		}
	}
}

void Seling::Move()
{
	/*if (m_isJump)
	{

		if (m_isLanding)
		{
			m_isJump = false;
			m_isLanding = false;
		}
	}*/
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
		XMFLOAT3 force(frontDirection.x * addForcePower, frontDirection.y * addForcePower, frontDirection.z * addForcePower);
		AddForce(force);
	}
	if (VoiceReciver::GetBack() || goBackKey.GetKey())
	{
		XMFLOAT3 force(frontDirection.x * -addForcePower, frontDirection.y * -addForcePower, frontDirection.z * -addForcePower);
		AddForce(force);
	}

	VoiceReciver::SetRight(false);
	VoiceReciver::SetLeft(false);
	VoiceReciver::SetFront(false);
	VoiceReciver::SetBack(false);

	XMFLOAT3 easeGoal(selingModel.each.rotation.x, selingModel.each.rotation.y, angle);
	selingModel.each.rotation = ShlomonMath::EaseInQuad(selingModel.each.rotation, easeGoal, 0.3f);
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
	if (Lenght(selingModel.each.position, StageObjects::goals.goalsPos.position) <= 2.0f)
	{
		isGoal = true;
	}
}

void Seling::SpringBorad()
{
	for (auto itr = springBorad.begin(); itr != springBorad.end(); ++itr)
	{
		(*itr)->Update((*this), (*itr)->m_partEach);
	}
}

void Seling::LoadKeys()
{
	goFrontKey.LoadKey("PlayerGoFront");
	goBackKey.LoadKey("PlayerGoBack");
	lookToRightKey.LoadKey("PlayerLookToRight");
	lookToLeftKey.LoadKey("PlayerLookToLeft");
}
