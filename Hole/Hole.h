#pragma once
#include <DirectXMath.h>
#include "../3DModel/Model.h"
#include "../DX12operator.h"
class HoleModel : public Model
{

};

class HoleModels
{
public:
	static HoleModel holeModel;
	static void Init();
};

class Hole
{
private:
	HoleModel hole;
	EachInfo each;
	bool isActive;
	int life;
	static const int MaxLife = 180;
public:
	void SetModel(HoleModel &hole){ this->hole = hole; }
	XMFLOAT3 GetPosition(){return ConvertXMVECTORtoXMFLOAT3(each.position); }
	int GetLife() {return life;}

	void Init(XMFLOAT3 &position);
	void Update();
	void Draw();
};

class Holes
{
public:
	static list<Hole> holes;
	static list<list<Hole>::iterator> deleteHoles;
public:
	static list<Hole> GetHoleList() { return holes; }
	static void Init();
	static void Update();
	static void Draw();

	static void AddHole(Hole &hole);
	static void DeleteHole();
};