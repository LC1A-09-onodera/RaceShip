#pragma once
#include "../3DModel/Model.h"

class WaterFaceModel : public Model
{
public:
	void Update();
	void PreDraw();
	void PostDraw();
	void Draw(EachInfo &each);
};

class WaterFace
{
	WaterFaceModel waterModel;

	EachInfo each;
	void LoadModel();
	void Init();
	void Update();
	void Draw();
};