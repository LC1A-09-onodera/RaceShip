#pragma once
#include "../3DModel/Model.h"

class Seling;
class Camera;
class SpringBoard
{
public:
	enum class Direction
	{
		RightToLeft,
		LeftToRight,
		UpToBottom,
		BottomToUp,
		NONE,
	};
	//â°
	float m_wid = 1.0f;
	//çÇÇ≥
	float m_hi = 1.0f;
	//èc
	float m_vert = 1.0f;
	EachInfo m_partEach;
private:
	static Model m_model;
	bool m_isHit = false;
	bool m_OldHit = false;
	Direction m_direction = Direction::RightToLeft;
	Direction m_hitDirection = Direction::NONE;
	
	//ó]âC
	bool isAfterTaste = false;
	float afterTaste = 0.0f;
	const float MaxAfterTaste = 1.0f;

public:
	void Update(Seling &seling, EachInfo &each);
	void Draw(BaseDirectX &baseDirectX);
	void Draw(BaseDirectX& baseDirectX, bool isR);
	void Draw(BaseDirectX& baseDirectX, Camera &isR);
	void Init(BaseDirectX &baseDirectX, SpringBoard::Direction direction);
	static void LoadModel(BaseDirectX & baseDirectX);
	bool Collition(Seling& seling,XMFLOAT3 thisPos);
};