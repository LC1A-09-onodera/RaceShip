#pragma once
#include "../3DModel/Model.h"

struct ConstSampleObject : public ConstBufferDataB0
{
public:
	UINT frameTime;
};

class SampleObject : public Model
{
public:
	void Init(int index);
	void Update();

	UINT frameTime;
	DirectX::XMFLOAT3 direction;
	bool isMove;
};