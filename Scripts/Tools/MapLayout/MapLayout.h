#pragma once
#include <list>
#include <vector>
#include <string>
#include <array>
#include <DirectXMath.h>
#include "../../3DModel/Model.h"

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

//�I�u�W�F�N�g�̖��O����
class MapObjectNames
{
	static string wall;
	static string goal;
	static string enemy;
};

//�}�b�v�G�f�B�^���ŉ��ςȂǂ��s���N���X
class MapEditorObject
{
public:
	EachInfo piece;
	bool isActive = false;
	bool OnCollisionMouse(float posX, float posY);
	void Init( const XMFLOAT3& position);
	string PositionToString();
};

//�O���ۑ��p�N���X
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
	static void LoadModels();
	static void LoadFile(const string path);
	static void Update(  const XMFLOAT3& f_mousePos);
	static void Draw();
	static void SetObject(const XMFLOAT3& position, const XMFLOAT3 & f_rotation);
	static void SetObjectLine(const XMFLOAT3& position);
	static bool ObjectCollision(const XMFLOAT3& f_mousePos);
	static bool OutputFile(const char *path);
	static void DeleteObjects();
	static void EraseObject();
};