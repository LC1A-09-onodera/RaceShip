#include "water.h"
#include "../DX12operator.h"
#include "../Shader/ShaderManager.h"
Model Water::ground;

Water::Water()
{
	isPolygon = false;
}

Water::~Water()
{
	
}

void Water::Init()
{
	
}

void Water::CreateWater()
{
	ground.CreateModel("ground", ShaderManager::playerShader);
	ground.mesh.vertices.clear();
	ground.mesh.indices.clear();
	ground.mesh.vertices.shrink_to_fit();
	ground.mesh.indices.shrink_to_fit();
	Vertex sample;
	WaterVert watervert;
	float length = 5.0f;
	//í∏ì_ÇÃê›íË
	sample.pos = { -1.0f, 1.0f, 0.0f };
	ground.mesh.vertices.push_back(sample);
	sample.pos = { -1.0f, -1.0f, 0.0f };
	ground.mesh.vertices.push_back(sample);
	sample.pos = { 1.0f, -1.0f, 0.0f };
	ground.mesh.vertices.push_back(sample);
	sample.pos = { 1.0f, 1.0f, 0.0f };
	ground.mesh.vertices.push_back(sample);
	//index
	ground.mesh.indices.push_back(0);
	ground.mesh.indices.push_back(1);
	ground.mesh.indices.push_back(1);
	ground.mesh.indices.push_back(2);
	ground.mesh.indices.push_back(2);
	ground.mesh.indices.push_back(3);
	ground.mesh.indices.push_back(3);
	ground.mesh.indices.push_back(0);
	ground.Init(0);
	for (int i = 0; i < Water::waterAmount; i++)
	{
		for (int j = 0;j < Water::waterAmount;j++)
		{
			each[j][i].CreateConstBuff0();
			each[j][i].CreateConstBuff1();
			each[j][i].position.m128_f32[0] = 2.0f * (i - Water::waterAmount / 2);
			each[j][i].position.m128_f32[1] = 2.0f * (j - Water::waterAmount / 2 + 1);
		}
	}
}

void Water::Update()
{
	
}

void Water::Draw()
{
	for (int i = 0; i < Water::waterAmount; i++)
	{
		for (int j = 0;j < Water::waterAmount;j++)
		{
			ground.Update(&each[j][i]);
			Draw3DObject(ground, -1, false);
		}
	}
}
