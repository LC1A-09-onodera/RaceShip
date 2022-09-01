#include "SpringBoard.h"
#include "../Shader/ShaderManager.h"
#include "../Player/Seling.h"

Model SpringBoard::m_model;

void SpringBoard::Update(Seling& seling)
{
	XMFLOAT3 selingPos = ConvertXMVECTORtoXMFLOAT3(seling.selingModel.each.position);
	XMFLOAT3 thisPos = ConvertXMVECTORtoXMFLOAT3(m_model.each.position);
	m_OldHit = m_isHit;
	if (Collition(seling, thisPos))
	{
		m_isHit = true;
	}
	else
	{
		m_hitDirection = Direction::NONE;
		if (m_OldHit && !m_isHit && !isAfterTaste)
		{
			isAfterTaste = true;
		}
		else if (isAfterTaste)
		{
			afterTaste += 0.002f;
		}
		if (afterTaste > MaxAfterTaste)
		{
			afterTaste = 0.0f;
			seling.m_isJump = false;
		}
		m_isHit = false;
	}
	if (!m_OldHit && m_isHit)
	{

	}
}

void SpringBoard::Draw(BaseDirectX& baseDirectX)
{
	m_partEach.scale = { m_wid, m_hi, m_vert };
	SpringBoard::m_model.Update(baseDirectX, &m_partEach);
	Draw3DObject(baseDirectX, SpringBoard::m_model);
}

void SpringBoard::Draw(BaseDirectX& baseDirectX, bool isR)
{
	m_partEach.position.m128_f32[1] = m_hi;
	m_partEach.scale = { m_wid, m_hi, m_vert };
	SpringBoard::m_model.Update(baseDirectX, &m_partEach, isR);
	Draw3DObject(baseDirectX, SpringBoard::m_model);
}

void SpringBoard::Draw(BaseDirectX& baseDirectX, Camera& isR)
{
	m_partEach.scale = { m_wid, m_hi, m_vert };
	m_partEach.position.m128_f32[1] = m_hi;
	SpringBoard::m_model.Update(baseDirectX, &m_partEach, isR);
	Draw3DObject(baseDirectX, SpringBoard::m_model);
}

void SpringBoard::LoadModel(BaseDirectX& baseDirectX)
{
	SpringBoard::m_model.CreateModel(baseDirectX, "SpringBorad", ShaderManager::playerShader);
}

void SpringBoard::Init(BaseDirectX& baseDirectX, SpringBoard::Direction direction)
{
	ConstInit<EachInfo>(m_partEach, baseDirectX.dev);
	m_direction = direction;
	float angle = 0.0f;
	if (direction == Direction::RightToLeft)
	{
		angle = 0.0f;
	}
	else if (direction == Direction::LeftToRight)
	{
		angle = 180.0f;
	}
	else if (direction == Direction::BottomToUp)
	{
		angle = 90.0f;
	}
	else if (direction == Direction::UpToBottom)
	{
		angle = 270.0f;
	}
	m_partEach.rotation.z = angle;
}

bool SpringBoard::Collition(Seling& seling, XMFLOAT3 thisPos)
{
	XMFLOAT3 playerPos = ConvertXMVECTORtoXMFLOAT3(seling.selingModel.each.position);
	float XLenght = playerPos.x - thisPos.x;
	float ZLenght = playerPos.z - thisPos.z;

	bool Hit = false;
	if (m_direction == Direction::LeftToRight || m_direction == Direction::RightToLeft)
	{
		Hit = abs(XLenght) < m_wid + 1.0f && abs(ZLenght) < m_vert + 2.0f;
	}
	else if (m_direction == Direction::UpToBottom || m_direction == Direction::BottomToUp)
	{
		Hit = abs(XLenght) < m_vert + 1.0f && abs(ZLenght) < m_wid + 2.0f;
	}
	if (!Hit) return false;

	m_isHit = true;
	bool widHit = false;
	bool vertHit = false;
	if (m_direction == Direction::LeftToRight || m_direction == Direction::RightToLeft)
	{
		widHit = abs(XLenght) < m_wid + 1.0f;
		vertHit = abs(ZLenght) < m_vert + 1.0f;
	}
	else if (m_direction == Direction::UpToBottom || m_direction == Direction::BottomToUp)
	{
		widHit = abs(XLenght) < m_vert + 1.0f;
		vertHit = abs(ZLenght) < m_wid + 1.0f;
	}
	//true¨right false¨left
	bool isRight = playerPos.x > thisPos.x + 0.001f;
	bool isLeft = playerPos.x < thisPos.x - 0.001f;
	//true¨front false¨rear
	bool isFront = playerPos.z > thisPos.z + 0.001f;
	bool isRear = playerPos.z < thisPos.z - 0.001f;

	//Š„‡
	float shidePart = 0.0f;
	float frontAndRearPart = 0.0f;
	if (!m_OldHit)
	{
		if (isRight && vertHit)
		{
			m_hitDirection = Direction::RightToLeft;
		}
		else if (isLeft && vertHit)
		{
			m_hitDirection = Direction::LeftToRight;
		}
		else if (isFront && widHit)
		{
			m_hitDirection = Direction::UpToBottom;
		}
		else if (isRear && widHit)
		{
			m_hitDirection = Direction::BottomToUp;
		}
	}
	const float sensityRL = 1.0f;
	const float sensityUB = 1.0f;
	const float selingHi = 0.25f;
	if (m_hitDirection == m_direction && m_direction == Direction::RightToLeft)
	{
		shidePart = (m_wid - XLenght) / 10.0f;
		seling.selingModel.each.position.m128_f32[1] = shidePart * m_hi * sensityRL + selingHi;
	}
	else if (m_hitDirection == m_direction && m_direction == Direction::LeftToRight)
	{
		shidePart = (m_wid - (-XLenght)) / 10.0f;
		seling.selingModel.each.position.m128_f32[1] = shidePart * m_hi * sensityRL + selingHi;
	}
	else if (m_hitDirection == m_direction && m_direction == Direction::UpToBottom)
	{
		
		float part = ShlomonMath::Clamp(m_wid - ZLenght, 0.0f, 100.0f);
		frontAndRearPart = (part) / 10.0f;
		seling.selingModel.each.position.m128_f32[1] = frontAndRearPart * m_hi * sensityUB + selingHi;
	}
	else if (m_hitDirection == m_direction && m_direction == Direction::BottomToUp)
	{
		float part = ShlomonMath::Clamp(m_wid - (-ZLenght), 0.0f, 100.0f);
		frontAndRearPart = (part) / 10.0f;
		seling.selingModel.each.position.m128_f32[1] = frontAndRearPart * m_hi * sensityUB + selingHi;
	}
	if (m_hitDirection != m_direction)
	{
		if (m_hitDirection == Direction::UpToBottom || m_hitDirection == Direction::BottomToUp)
		{
			seling.addForce.z = -seling.addForce.z / 6.0f;
		}
		if (m_hitDirection == Direction::LeftToRight || m_hitDirection == Direction::RightToLeft)
		{
			seling.addForce.x = -seling.addForce.x / 6.0f;
		}
	}
	return true;
}
