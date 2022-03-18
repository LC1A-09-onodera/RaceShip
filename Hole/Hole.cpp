#include "Hole.h"
#include "../Shader/ShaderManager.h"

list<Hole> Holes::holes;
list<list<Hole>::iterator> Holes::deleteHoles;
HoleModel HoleModels::holeModel;

void Hole::Init(XMFLOAT3 &position)
{
	SetModel(HoleModels::holeModel);
	each.CreateConstBuff0();
	each.CreateConstBuff1();
	each.position = ConvertXMFLOAT3toXMVECTOR(position);
	isActive = true;
	life = MaxLife;
}

void Hole::Update()
{
	if (!isActive) return;
	if (life <= 0) return;
	life--;
}

void Hole::Draw()
{
	hole.Update(&each);
	Draw3DObject(hole);
}

void Holes::Init()
{
	if (holes.size() == 0)return;
	holes.clear();
	if (deleteHoles.size() == 0)return;
	deleteHoles.clear();
}

void Holes::Update()
{
	if (holes.size() <= 0) return;
	auto itr = holes.begin();
	for (; itr != holes.end(); ++itr)
	{
		itr->Update();
	}
	DeleteHole();
}

void Holes::Draw()
{
	if (holes.size() <= 0) return;
	auto itr = holes.begin();
	for (; itr != holes.end(); ++itr)
	{
		itr->Draw();
	}
}

void Holes::AddHole(Hole &hole)
{
	holes.push_back(hole);
}

void Holes::DeleteHole()
{
	if (holes.size() <= 0) return;
	auto itr = holes.begin();
	for (; itr != holes.end(); ++itr)
	{
		if (itr->GetLife() <= 0)
		{
			deleteHoles.push_back(itr);
		}
	}
	if (deleteHoles.size() <= 0) return;
	auto deleteItr = deleteHoles.begin();
	for (; deleteItr != deleteHoles.end(); ++deleteItr)
	{
		holes.erase(*deleteItr);
	}
	deleteHoles.clear();
}

void HoleModels::Init()
{
	holeModel.CreateModel("Hole", ShaderManager::playerShader);
}
