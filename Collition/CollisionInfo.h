#pragma once
#include <DirectXMath.h>

class Model;
class BaseCollider;

using namespace DirectX;

struct CollisionInfo
{
public:
	CollisionInfo(Model* model, BaseCollider* collider, const XMVECTOR& inter)
	{
		this->model = model;
		this->collider = collider;
		this->inter = inter;
	}
	Model * model;
	BaseCollider *collider = nullptr;
	XMVECTOR inter;
};