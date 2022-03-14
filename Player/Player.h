#pragma once
//#include "../3DModel/Model.h"
#include "../sampleObject/sampleObject.h"
#include "../Collition/CollisionPrimitive.h"
#include "../Sound/Sound.h"
#include "../water/water.h"
using namespace DirectX;

class Enemy;

static XMFLOAT3 VecNormaliz(XMFLOAT3& vector)
{
	float leg = sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	vector.x = vector.x / leg;
	vector.y = vector.y / leg;
	vector.z = vector.z / leg;
	return vector;
}

enum PlayerAction
{
	P_MOVE, P_STAY
};

enum class Direction
{
	RIGHT, LEFT,
};

//struct WaterVert
//{
//	int index;
//	int nearIndex[4];
//	float moveSpeed;
//	float addSpeed;
//	bool moveDirection;
//	XMFLOAT3 nextPosition;
//};

class Player final
{
private:
	Player();
	~Player();
	/*std::vector<WaterVert> waterVert;*/
	//Model ground;
	Water water;
	static const int WallNum = 500;
	Model goal;
	XMFLOAT3 pos;
	XMFLOAT3 direction = {0, 0, 1};

	float moveSpeedX;
	float moveSpeedY;
	bool isDie;
	bool isGoal;
	int HP;
	float speedEase;
	XMFLOAT3 wallToDirection;
	/*static const int vertNum = 250;*/
	static const int R = 1;
	static const int MaxWS = 1;
	static const int MaxAD = 3;

	XMFLOAT3 directionMove;
	XMFLOAT3 directionR;
	XMFLOAT2 normalSpeed;
	float leng;

	int goalAfterCount = 0;

	static const int moveCountMax = 100;
	int moveCount;

	//SoundData BGM;
public:
	Player(const Player &obj) = delete;
	Player &operator=(const Player &obj) = delete;
	static Player *GetPlayer();
	//èâä˙âª
	void Init();
	//çXêV
	void Update();
	//ï`âÊ
	void Draw();
	//
	void ReSet();
	//
	XMFLOAT3 GetPos();
	//
	void Move();

	XMVECTOR oldPlayerPos;
	Box2D playerCollision;
	SampleObject player;
	int particleTime;
	bool isParticle;
};