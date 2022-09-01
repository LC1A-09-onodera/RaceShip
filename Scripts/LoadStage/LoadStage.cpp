#include "LoadStage.h"
#include <fstream>
#include <sstream>

list<LoadStage::LoadInfo> LoadStage::wallPosition;
list<LoadStage::LoadInfo> LoadStage::goalPosition;
list<LoadStage::LoadInfo> LoadStage::enemyPosition;
LoadStage::LoadInfo LoadStage::playerPos;
void LoadStage::LoadStages(string stagePath)
{
	ifstream file;
	file.open(stagePath);
	if (file.fail())
	{
		assert(0);
	}
	string line;

	while (getline(file, line))
	{
		istringstream line_stream(line);

		string key;
		getline(line_stream, key, ' ');
		
		LoadStage::LoadInfo info;
		line_stream >> info.position.x;
		line_stream >> info.position.y;
		line_stream >> info.position.z;
		line_stream >> info.rotation.x;
		line_stream >> info.rotation.y;
		line_stream >> info.rotation.z;
		if (key == "wall")
		{
			wallPosition.push_back(info);
		}
		else if (key == "goal")
		{
			goalPosition.push_back(info);
		}
		else if (key == "enemy")
		{
			enemyPosition.push_back(info);
		}
		else if (key == "player")
		{
			playerPos = info;
		}
	}
}

void LoadStage::LoadStages(string stagePath, Seling& selingPos)
{
	ifstream file;
	file.open(stagePath);
	if (file.fail())
	{
		assert(0);
	}
	string line;
	wallPosition.clear();
	goalPosition.clear();
	enemyPosition.clear();

	while (getline(file, line))
	{
		istringstream line_stream(line);

		string key;
		getline(line_stream, key, ' ');

		LoadStage::LoadInfo info;
		line_stream >> info.position.x;
		line_stream >> info.position.y;
		line_stream >> info.position.z;
		line_stream >> info.rotation.x;
		line_stream >> info.rotation.y;
		line_stream >> info.rotation.z;
		if (key == "wall")
		{
			wallPosition.push_back(info);
		}
		else if (key == "goal")
		{
			goalPosition.push_back(info);
		}
		else if (key == "enemy")
		{
			enemyPosition.push_back(info);
		}
		else if (key == "player")
		{
			selingPos.selingModel.each.position.m128_f32[0] = info.position.x;
			selingPos.selingModel.each.position.m128_f32[1] = info.position.y;
			selingPos.selingModel.each.position.m128_f32[2] = info.position.z;
			selingPos.selingModel.each.rotation.x = info.rotation.x;
			selingPos.selingModel.each.rotation.y = info.rotation.z;
			selingPos.selingModel.each.rotation.z = info.rotation.y;
		}
	}
}
