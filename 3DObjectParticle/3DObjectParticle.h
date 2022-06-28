#pragma once
#include "../3DModel/Model.h"

class ObjectParticle : public Model
{

};
class ObjectParticle3D
{
public:
	enum class ParticleType
	{
		Exprotion,
		Converge,
		TITLE,
		Swell,
		Target,
		Tornado,
		Born,
		LinkStart,

	};
private:
	const int Life = 120;

	XMFLOAT3 speed = { 0, 0, 0 };
	XMFLOAT3 acc = { 0, 0, 0 };
	XMFLOAT3 addRotation = { 0, 0, 0 };
	XMFLOAT3 startPosition = { 0, 0, 0 };
	XMFLOAT3 endPosition = { 0, 0, 0 };
	const float addTime = 0.002f;
	float easeTime = 0.0f;
	bool isSize = false;
	ParticleType type = ParticleType::Exprotion;
	float angle = 0.0f;
public:
	EachInfo each;
	void Add(BaseDirectX& baseDirectX, XMFLOAT3& emitter, ParticleType type);
	void Update();
	void Draw(BaseDirectX& baseDirectX, ObjectParticle& object);
	void InitExprotion(BaseDirectX& baseDirectX, XMFLOAT3& emitter);
	void InitConverge(BaseDirectX& baseDirectX, XMFLOAT3& emitter);
	void InitTitle(BaseDirectX& baseDirectX, XMFLOAT3& emitter);
	void InitSwell(BaseDirectX& baseDirectX, XMFLOAT3& emitter);
	void InitTarget(BaseDirectX& baseDirectX, XMFLOAT3& emitter);
	void InitTornado(BaseDirectX& baseDirectX, XMFLOAT3& emitter);
	void InitBorn(BaseDirectX& baseDirectX, XMFLOAT3& emitter);
	void InitLink();
	void UpdateExprotion();
	void UpdateConverge();
	void UpdateTitle();
	void UpdateSwell();
	void UpdateTarget();
	void UpdateTornado();
	void UpdateBorn();
	void UpdateLink();
	int time = 0;
};

class ObjectParticleInfo
{
public:
	list<ObjectParticle3D> particles;
	list<list<ObjectParticle3D>::iterator> deleteItr;
	ObjectParticle object;
	void Init(BaseDirectX& baseDirectX, XMFLOAT3& emitter, int count, ObjectParticle3D::ParticleType type);
	void Update();
	void Draw(BaseDirectX &baseDirectX, ObjectParticle& object);
	void DeleteAllParticle();

};

class ObjectParticles
{
public:
	static ObjectParticleInfo triangle;
	static void LoadModels(BaseDirectX& baseDirectX);
	static void Update();
	static void Draw(BaseDirectX& baseDirectX);
	static void DeleteAllParticles();
};