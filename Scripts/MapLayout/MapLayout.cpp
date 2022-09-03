#include <sstream>
#include "MapLayout.h"
#include "../BaseDirectX/DX12operator.h"
#include "../BaseDirectX/Input.h"
#include "../Camera/Camera.h"
#include "../Shader/ShaderManager.h"
#include <fstream>

string MapObjectNames::wall = "wall";
string MapObjectNames::goal = "goal";
string MapObjectNames::enemy = "enemy";

list<MapEditorObject> MapEditorObjects::wall;
MapEditorObject MapEditorObjects::goal;
list<MapEditorObject> MapEditorObjects::enemy;
list<MapEditorObject> MapEditorObjects::springBorads;
MapEditorObject MapEditorObjects::player;

Model MapEditorObjects::wallModel;
Model MapEditorObjects::goalModel;
Model MapEditorObjects::enemyModel;
Model MapEditorObjects::playerModel;
Model MapEditorObjects::sprinfBoradModel;
MapObjects MapEditorObjects::activeType;

XMFLOAT3 MapEditorObjects::lineMousePos;
bool MapEditorObjects::isLinePut;
const int MapEditorObjects::MapW = 25;
const int MapEditorObjects::MapH = 25;
list<pair<list<MapEditorObject>::iterator, MapObjects>> MapEditorObjects::andoList;

bool MapEditorObject::OnCollisionMouse(float posX, float posY)
{
	const float mapObjectR = 0.53f;
	XMFLOAT3 pos;
	pos.x = posX;
	pos.y = posY;
	pos.z = 0.0f;
	if (Lenght(piece.position, pos) < mapObjectR)
	{
		return true;
	}
	return false;
}

void MapEditorObject::Init(BaseDirectX& baseDirectX, XMFLOAT3& position)
{
	ConstInit(piece, baseDirectX.dev);
	piece.position = ConvertXMFLOAT3toXMVECTOR(position);
	piece.rotation.y = 0.0f;
	isActive = true;
}

string MapEditorObject::PositionToString()
{
	ostringstream oss1;
	oss1 << -piece.position.m128_f32[0];
	ostringstream oss2;
	oss2 << piece.position.m128_f32[1];
	ostringstream oss3;
	oss3 << piece.position.m128_f32[2];
	ostringstream oss4;
	oss4 << piece.rotation.x;
	ostringstream oss5;
	oss5 << piece.rotation.y;
	ostringstream oss6;
	oss6 << piece.rotation.z;
	string str;
	str = oss1.str() + " " + oss2.str() + " " + oss3.str() + " " + oss4.str() + " " + oss5.str() + " " + oss6.str();
	return str;
}

void MapEditorObjects::LoadModels(BaseDirectX& baseDirectX)
{
	wallModel.CreateModel(baseDirectX, "MapWall", ShaderManager::playerShader);
	goalModel.CreateModel(baseDirectX, "MapGoal", ShaderManager::playerShader);
	enemyModel.CreateModel(baseDirectX, "MapEnemy", ShaderManager::playerShader);
	playerModel.CreateModel(baseDirectX, "Mapplayer", ShaderManager::playerShader);
	sprinfBoradModel.CreateModel(baseDirectX, "MapSB", ShaderManager::playerShader);
}

void MapEditorObjects::LoadFile(string path)
{
}

void MapEditorObjects::Update(BaseDirectX& baseDirectX, XMFLOAT3& f_mousePos)
{
	if (Input::KeyTrigger(DIK_1))
	{
		activeType = MapObjects::WALL;
	}
	if (Input::KeyTrigger(DIK_2))
	{
		activeType = MapObjects::GOAL;
	}
	if (Input::KeyTrigger(DIK_3))
	{
		activeType = MapObjects::PLAYER;
	}
	if (Input::KeyTrigger(DIK_4))
	{
		activeType = MapObjects::SpringBorad;
	}
	if (Input::KeyTrigger(DIK_0))
	{
		activeType = MapObjects::NONE;
	}
	if (Input::MouseTrigger(MouseButton::LBUTTON))
	{
		isLinePut = true;
		if (!ObjectCollision(f_mousePos))
		{
			SetObject(baseDirectX, f_mousePos);
		}
	}
	if (Input::MouseTrigger(MouseButton::RBUTTON))
	{
		ObjectCollision(f_mousePos);
	}
	else if (!Input::Mouse(MouseButton::LBUTTON))
	{
		isLinePut = false;
	}
	if (!Input::MouseTrigger(MouseButton::LBUTTON) && Input::Mouse(MouseButton::LBUTTON) && isLinePut)
	{
		SetObjectLine(baseDirectX, f_mousePos);
	}
	/*if (Input::Key(DIK_D))
	{
		auto itr = wall.begin();
		itr->piece.position.m128_f32[0] += 1.0f;
	}*/
	if (Input::Key(DIK_LCONTROL) && Input::KeyTrigger(DIK_Z))
	{
		EraseObject();
	}
}

void MapEditorObjects::Draw(BaseDirectX& baseDirectX)
{
	for (auto itr = wall.begin(); itr != wall.end(); ++itr)
	{
		wallModel.Update(baseDirectX, &itr->piece);
		Draw3DObject(baseDirectX, wallModel);
	}
	for (auto itr = springBorads.begin(); itr != springBorads.end(); ++itr)
	{
		sprinfBoradModel.Update(baseDirectX, &itr->piece);
		Draw3DObject(baseDirectX, sprinfBoradModel);
	}
	if (goal.isActive)
	{
		goalModel.Update(baseDirectX, &goal.piece);
		Draw3DObject(baseDirectX, goalModel);
	}
	for (auto itr = enemy.begin(); itr != enemy.end(); ++itr)
	{
		enemyModel.Update(baseDirectX, &itr->piece);
		Draw3DObject(baseDirectX, enemyModel);
	}
	if (player.isActive)
	{
		playerModel.Update(baseDirectX, &player.piece);
		Draw3DObject(baseDirectX, playerModel);
	}
}

void MapEditorObjects::SetObject(BaseDirectX& baseDirectX, XMFLOAT3& position)
{
	if (position.x > MapW || position.x < -MapW || position.y > MapH || position.y < -MapH)
	{
		return;
	}
	//�X�N���[���n
	POINT mouse = WindowsAPI::GetMousePos();
	XMFLOAT2 mouseFloat;
	mouseFloat = XMFLOAT2(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
	if (mouseFloat.x < 0.0f || mouseFloat.x > WindowsAPI::window_width || mouseFloat.y < 0.0f || mouseFloat.y > WindowsAPI::window_height)
	{
		return;
	}
	MapEditorObject object;
	object.Init(baseDirectX, position);
	lineMousePos = position;
	if (activeType == MapObjects::WALL)
	{
		MapEditorObjects::wall.push_back(object);
		auto itr = wall.end();
		--itr;
		pair<list<MapEditorObject>::iterator, MapObjects > ando = { itr, activeType };
		andoList.push_back(ando);
	}
	else if (activeType == MapObjects::SpringBorad)
	{
		MapEditorObjects::springBorads.push_back(object);
		auto itr = springBorads.end();
		--itr;
		pair<list<MapEditorObject>::iterator, MapObjects> ando = {itr ,activeType};
		andoList.push_back(ando);
	}
	else if (activeType == MapObjects::GOAL)
	{
		goal = object;
	}
	else if (activeType == MapObjects::PLAYER)
	{
		player = object;
	}
}

void MapEditorObjects::SetObjectLine(BaseDirectX& baseDirectX, XMFLOAT3& position)
{
	if (position.x > MapW || position.x < -MapW || position.y > MapH || position.y < -MapH)
	{
		return;
	}
	const float MapObjectR = 1.0f;
	if (lineMousePos.x + MapObjectR < position.x)
	{
		lineMousePos.x = lineMousePos.x + MapObjectR;
	}
	else if (lineMousePos.x - MapObjectR > position.x)
	{
		lineMousePos.x = lineMousePos.x - MapObjectR;
	}
	else if (lineMousePos.y + MapObjectR < position.y)
	{
		lineMousePos.y = lineMousePos.y + MapObjectR;
	}
	else if (lineMousePos.y - MapObjectR > position.y)
	{
		lineMousePos.y = lineMousePos.y - MapObjectR;
	}
	else
	{
		return;
	}
	SetObject(baseDirectX, lineMousePos);
}

bool MapEditorObjects::ObjectCollision(XMFLOAT3& f_mousePos)
{
	for (auto itr = wall.begin(); itr != wall.end(); ++itr)
	{
		if (itr->OnCollisionMouse(static_cast<float>(f_mousePos.x), static_cast<float>(f_mousePos.y)))
		{
			if (Input::MouseTrigger(MouseButton::LBUTTON))
			{
				wall.erase(itr);
			}
			else if (Input::MouseTrigger(MouseButton::RBUTTON))
			{
				itr->piece.rotation.x += 90.0f;
			}

			lineMousePos = f_mousePos;
			return true;
		}
	}

	for (auto itr = springBorads.begin(); itr != springBorads.end(); ++itr)
	{
		if (itr->OnCollisionMouse(static_cast<float>(f_mousePos.x), static_cast<float>(f_mousePos.y)))
		{
			if (Input::MouseTrigger(MouseButton::LBUTTON))
			{
				springBorads.erase(itr);
			}
			else if (Input::MouseTrigger(MouseButton::RBUTTON))
			{
				itr->piece.rotation.x += 90.0f;
			}

			lineMousePos = f_mousePos;
			return true;
		}
	}

	if (goal.OnCollisionMouse(static_cast<float>(f_mousePos.x), static_cast<float>(f_mousePos.y)))
	{
		if (Input::MouseTrigger(MouseButton::RBUTTON))
		{
			goal.piece.rotation.x += 90.0f;
		}
		lineMousePos = f_mousePos;
		return true;
	}

	for (auto itr = enemy.begin(); itr != enemy.end(); ++itr)
	{
		if (itr->OnCollisionMouse(static_cast<float>(f_mousePos.x), static_cast<float>(f_mousePos.y)))
		{
			if (Input::MouseTrigger(MouseButton::LBUTTON))enemy.erase(itr);
			else if (Input::MouseTrigger(MouseButton::RBUTTON))
			{
				itr->piece.rotation.x += 90.0f;
			}
			lineMousePos = f_mousePos;
			return true;
		}
	}
	if (player.OnCollisionMouse(static_cast<float>(f_mousePos.x), static_cast<float>(f_mousePos.y)))
	{
		if (Input::MouseTrigger(MouseButton::RBUTTON))
		{
			player.piece.rotation.x += 90.0f;
		}
		lineMousePos = f_mousePos;
		return true;
	}
	return false;
}

bool MapEditorObjects::OutputFile(const char* path)
{
	string fileName = path;
	ofstream ofs(fileName);
	if (!ofs) return false;

	if (!goal.isActive || !player.isActive)
	{
		return false;
	}
	//ofs << "wall" << std::endl;
	for (auto itr = wall.begin(); itr != wall.end(); ++itr)
	{
		ofs << "wall " + itr->PositionToString() << std::endl;
	}
	for (auto itr = springBorads.begin(); itr != springBorads.end(); ++itr)
	{
		ofs << "SpringBorad " + itr->PositionToString() << std::endl;
	}
	if (goal.isActive)
	{
		ofs << "goal " + goal.PositionToString() << std::endl;
	}
	if (player.isActive)
	{
		ofs << "player " + player.PositionToString() << std::endl;
	}
	return true;
}

void MapEditorObjects::DeleteObjects()
{
	wall.clear();
	springBorads.clear();
	goal.isActive = false;
	player.isActive = false;
}

void MapEditorObjects::EraseObject()
{
	if (andoList.size() < 1)return;
	auto eraseObject = andoList.end();
	--eraseObject;
	if (eraseObject->second == MapObjects::WALL)
	{
		wall.erase(eraseObject->first);
	}
	if (eraseObject->second == MapObjects::SpringBorad)
	{
		springBorads.erase(eraseObject->first);
	}
	andoList.erase(eraseObject);
}
