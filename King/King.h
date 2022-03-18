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
	XMFLOAT3 windDirection;
	bool isWind;
public:
	void LoadModel();
	KingModel GetModel() { return king; }
	XMFLOAT3 GetPosition(){return ConvertXMVECTORtoXMFLOAT3(this->king.each.position); }
	void SetPosition(XMFLOAT3 &position);
	int GetHP(){ return HP; }

	void Init(int HP = 10);
	void Update();
	void Draw();
};