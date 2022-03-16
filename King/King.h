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
	XMFLOAT3 GetPosition(){return ConvertXMVECTORtoXMFLOAT3(king.each.position); }
	void SetPosition(XMFLOAT3 &position);
	int GetHP(){ return HP; }

	void Init(int HP = 10);
	void Update();
	void Draw();
};
