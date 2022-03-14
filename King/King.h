#pragma once
#include "../3DModel/Model.h"
#include "../DX12operator.h"

class KingModel : public Model
{

};

class King
{
private:
	KingModel king;
	int HP;
public:
	XMFLOAT3 GetPosition(){return ConvertXMVECTORtoXMFLOAT3(king.position); }
	XMFLOAT3 SetPosition(XMFLOAT3 position);
	int GetHP();
};