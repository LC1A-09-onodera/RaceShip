#pragma once
#include <DirectXMath.h>
#include "../3DModel/Model.h"
#include "../DX12operator.h"
class HoleModel : Model
{

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
	XMFLOAT3 GetPOsition(){return ConvertXMVECTORtoXMFLOAT3(each.position); }
	int GetLife() {return life;}

	void Init(XMFLOAT3 position);

};