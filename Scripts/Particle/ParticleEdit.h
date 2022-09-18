#pragma once
#include "../BaseDirectX/BaseDirectX.h"
#include "../Particle/Particle3D.h"
#include <list>

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std;

class ParticleEach
{
public:
	XMFLOAT3 m_position;
	float scale;
	enum ParticleType
	{
		Normal,
		Easeeing,
		Loop,
	};
	ParticleType m_type;
	//Normal
	XMFLOAT3 m_vector;
	XMFLOAT3 m_acc;
	//Ease
	float m_time;
	XMFLOAT3 m_endPos;
	XMFLOAT3 m_startPos;
	//
	XMFLOAT3 m_midPos;

	void Init();
	void Update();
	void Draw();
};

class ParticleEachDeta
{
	XMFLOAT3 m_emitterPos;
	list<ParticleEach*> m_particles;
	ParticleIndi m_particleSprite;
	void Init();
	void Update();
	void Draw();
};

class ParticleEditManager
{
	static void Update();
	static void Init();
	static void Draw();
};