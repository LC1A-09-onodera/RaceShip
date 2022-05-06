#pragma once
#include "../3DModel/Model.h"
#include "../Collition/CollisionPrimitive.h"

//struct Wall
//{
//	Model wall;
//	Box2D collision;
//	bool isActive;
//};
struct WaterVert
{
	int index;
	int nearIndex[4];
	float moveSpeed;
	float addSpeed;
	bool moveDirection;
	XMFLOAT3 nextPosition;
};
class Water
{
public:
	Water();
	~Water();
	static Model ground;
	static const int waterAmount = 8;
	EachInfo each[waterAmount][waterAmount];
	static const int vertNum = 5;
	static const int vertNumX = 4;
	static const int R = 5;
	static const int GroundCount = 10;
	
	bool isPolygon;
	void Init();
	void CreateWater();
	void Update();
	void Draw();
	
	void VertexMove();
	void GetNearWaterVert();
	bool WaterVertCollision(XMFLOAT3& playerPos, XMFLOAT3& waterVert);
};