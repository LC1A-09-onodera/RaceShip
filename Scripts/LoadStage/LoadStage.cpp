#include "LoadStage.h"
#include <fstream>
#include <sstream>

list<XMFLOAT3> LoadStage::wallPosition;
list<XMFLOAT3> LoadStage::goalPosition;
list<XMFLOAT3> LoadStage::enemyPosition;

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
		
		XMFLOAT3 pos;
		line_stream >> pos.x;
		line_stream >> pos.y;
		line_stream >> pos.z;
		if (key == "wall")
		{
			wallPosition.push_back(pos);
		}
		else if (key == "goal")
		{
			goalPosition.push_back(pos);
		}
		else if (key == "enemy")
		{
			enemyPosition.push_back(pos);
		}
	}
}
