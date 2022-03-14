#include "water.h"
#include "../DX12operator.h"
#include "../Player/Player.h"
Water::Water()
{
	isPolygon = false;
}

Water::~Water()
{
	
}

void Water::Init()
{
	for (int i = 2; i <= vertNum - 1; i++)
	{
		for (int j = 1; j < vertNum - 1; j++)
		{
			for (int k = 0;k < GroundCount;k++)
			{
				ground[k].mesh.vertices[j + (vertNum * i)].pos.y = 0;
			}
			waterVert[((i - 1) * vertNum) + j].addSpeed = 0;
		}
	}
}

void Water::CreateWater()
{
	for (int k = 0;k < GroundCount;k++)
	{
		ground[k].shader.vsBlob = LoadShader(L"Resource/HLSL/OBJVertexShader.hlsl", "vs_5_0");
		ground[k].shader.psBlob = LoadShader(L"Resource/HLSL/OBJPixelShader.hlsl", "ps_5_0");
		ground[k].CreateModel("water", true);
		ground[k].mesh.vertices.clear();
		ground[k].mesh.indices.clear();
		ground[k].mesh.vertices.shrink_to_fit();
		ground[k].mesh.indices.shrink_to_fit();

		Vertex sample;
		WaterVert watervert;
		float length = 5.0f;
		//’¸“_‚ÌÝ’è
		
		sample.pos = { -7.5f, 0, -7.5f};
		ground[k].mesh.vertices.push_back(sample);
		sample.pos = { 7.5f, 0, -7.5f };
		ground[k].mesh.vertices.push_back(sample);
		sample.pos = { 7.5f, 0, 12.5f };
		ground[k].mesh.vertices.push_back(sample);
		sample.pos = { -7.5f, 0, 12.5f };
		ground[k].mesh.vertices.push_back(sample);
		sample.pos = { -2.5f, 0, -7.5f };
		ground[k].mesh.vertices.push_back(sample);
		sample.pos = { -2.5f, 0, 12.5f };
		ground[k].mesh.vertices.push_back(sample);
		sample.pos = { 2.5f, 0, -7.5f };
		ground[k].mesh.vertices.push_back(sample);
		sample.pos = { 2.5f, 0, 12.5f };
		ground[k].mesh.vertices.push_back(sample);
		sample.pos = { -7.5f, 0, -2.5f };
		ground[k].mesh.vertices.push_back(sample);
		sample.pos = { 7.5f, 0, -2.5f };
		ground[k].mesh.vertices.push_back(sample);
		sample.pos = { -7.5f, 0, 2.5f };
		ground[k].mesh.vertices.push_back(sample);
		sample.pos = { 7.5f, 0, 2.5f };
		ground[k].mesh.vertices.push_back(sample);
		sample.pos = { -7.5f, 0, 7.5f };
		ground[k].mesh.vertices.push_back(sample);
		sample.pos = { 7.5f, 0, 7.5f };
		ground[k].mesh.vertices.push_back(sample);
		//index
		ground[k].mesh.indices.push_back(0);
		ground[k].mesh.indices.push_back(1);
		ground[k].mesh.indices.push_back(1);
		ground[k].mesh.indices.push_back(2);
		ground[k].mesh.indices.push_back(2);
		ground[k].mesh.indices.push_back(3);
		ground[k].mesh.indices.push_back(3);
		ground[k].mesh.indices.push_back(0);
		ground[k].mesh.indices.push_back(4);
		ground[k].mesh.indices.push_back(5);
		ground[k].mesh.indices.push_back(6);
		ground[k].mesh.indices.push_back(7);
		ground[k].mesh.indices.push_back(8);
		ground[k].mesh.indices.push_back(9);
		ground[k].mesh.indices.push_back(10);
		ground[k].mesh.indices.push_back(11);
		ground[k].mesh.indices.push_back(12);
		ground[k].mesh.indices.push_back(13);
		ground[k].Init(0);
	}
	for (int i = 0; i < GroundCount; i++)
	{
		ground[i].position.m128_f32[2] = 15 * i;
	}
}

void Water::Update()
{
	for (int i = 0;i < GroundCount;i++)
	{
		ground[i].Update();
	}
}

void Water::Draw()
{
	for (int i = 0; i < GroundCount; i++)
	{
		Draw3DObject(ground[i], - 1, false);
	}
}
