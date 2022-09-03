#pragma once
#include <list>
#include <vector>
#include <string>
#include <array>
#include <DirectXMath.h>
#include "../3DModel/Model.h"

using namespace std;
using namespace DirectX;

enum class MapObjects
{
	NONE,
	WALL,
	GOAL,
	ENEMY,
	PLAYER,
	SpringBorad,
};

//オブジェクトの名前たち
class MapObjectNames
{
	static string wall;
	static string goal;
	static string enemy;
};

//マップエディタ内で改変などを行うクラス
class MapEditorObject
{
public:
	EachInfo piece;
	bool isActive = false;
	bool OnCollisionMouse(float posX, float posY);
	void Init(BaseDirectX &baseDirectX, XMFLOAT3& position);
	string PositionToString();
};

//外部保存用クラス
class MapPreservation
{
	XMFLOAT3 position;

};

class MapEditorObjects
{
	static list<MapEditorObject> wall;
	static MapEditorObject goal;
	static list<MapEditorObject> enemy;
	static list<MapEditorObject> springBorads;
	static MapEditorObject player;
	static Model wallModel;
	static Model goalModel;
	static Model enemyModel;
	static Model playerModel;
	static Model sprinfBoradModel;
	static XMFLOAT3 lineMousePos;
	static bool isLinePut;

	static list<pair<list<MapEditorObject>::iterator, MapObjects>> andoList;
public:
	static const int MapW;
	static const int MapH;
	static MapObjects activeType;
	static void LoadModels(BaseDirectX &baseDirectX);
	static void LoadFile(string path);
	static void Update(BaseDirectX& baseDirectX, XMFLOAT3& f_mousePos);
	static void Draw(BaseDirectX& baseDirectX);
	static void SetObject(BaseDirectX& baseDirectX, XMFLOAT3& position);
	static void SetObjectLine(BaseDirectX& baseDirectX, XMFLOAT3& position);
	static bool ObjectCollision(XMFLOAT3& f_mousePos);
	static bool OutputFile(const char *path);
	static void DeleteObjects();
	static void EraseObject();
};
