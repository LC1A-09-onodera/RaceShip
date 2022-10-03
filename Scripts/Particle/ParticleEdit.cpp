#include "ParticleEdit.h"
#include <string>
#include <fstream>
#include <string>
#include <sstream>
#include <assert.h>
#include "../imgui/ImguiControl.h"

using namespace std;

void ParticleExport::CreatePartileFile(const char* f_fileName)
{
	string type = "";
	string count = "";
	string speed = "";
	string speedDiff = "";
	string acc = "";
	string startPos = "";
	string endPos = "";
	string life = "";
	string easeType = "";
	string startSize = "";
	string endSize = "";
	string spornSpan = "";
	string spornArea = "";
	string emitterLife = "";
	count = to_string(Imgui::particleCount);
	life = to_string(Imgui::particleLife);
	startSize = to_string(Imgui::particleStartSize);
	endSize = to_string(Imgui::particleEndSize);
	spornArea = to_string(Imgui::particleSpornArea[0]) + "," + to_string(Imgui::particleSpornArea[1]) + "," + to_string(Imgui::particleSpornArea[2]);
	emitterLife = to_string(Imgui::emitterLife);
	spornSpan = to_string(Imgui::particleSpornSpan);
	
	switch (Imgui::particleType)
	{
	case Imgui::ParticleType::Normal:
		type = "Normal";
		break;
	case Imgui::ParticleType::Easeeing:
		type = "Easeeing";
		break;
	case Imgui::ParticleType::Lerp:
		type = "Lerp";
		break;
	default:
		break;
	}
	string particleFileName = f_fileName;
	//ÉtÉ@ÉCÉãÇçÏÇÈ
	string saveFileName = "Resource/TextData/Particle/" + particleFileName + ".csv";
	ofstream ofs(saveFileName);
	if (!ofs) return;
	ofs << "Type,"         + type << endl;
	ofs << "Count,"        + count << endl;
	ofs << "EmitterLife,"  + emitterLife << endl;
	ofs << "ParticleLife," + life << endl;
	ofs << "SpornSpan,"    + spornSpan << endl;
	ofs << "SpornArea,"    + spornArea << endl;
	ofs << "StartSize,"    + startSize << endl;
	ofs << "EndSize,"      + endSize << endl;
	switch (Imgui::particleType)
	{
	case Imgui::ParticleType::Normal:
		speed = to_string(Imgui::particleSpeed[0]) + "," + to_string(Imgui::particleSpeed[1]) + "," + to_string(Imgui::particleSpeed[2]);
		speedDiff = to_string(Imgui::particleSpeedDiff[0]) + "," + to_string(Imgui::particleSpeedDiff[1]) + "," + to_string(Imgui::particleSpeedDiff[2]);
		acc = to_string(Imgui::particleAcc[0]) + "," + to_string(Imgui::particleAcc[1]) + "," + to_string(Imgui::particleAcc[2]);
		ofs << "Speed," + speed << endl;
		ofs << "SpeedDiff," + speedDiff << endl;
		ofs << "Acc," + acc << endl;
		break;
	case Imgui::ParticleType::Easeeing:
		type = "Easeeing";
		startPos = to_string(Imgui::particleStartPosition[0]) + "," + to_string(Imgui::particleStartPosition[1]) + "," + to_string(Imgui::particleStartPosition[2]);
		endPos = to_string(Imgui::particleEndPosition[0]) + "," + to_string(Imgui::particleEndPosition[1]) + "," + to_string(Imgui::particleEndPosition[2]);
		ofs << "StartPosition," + startPos << endl;
		ofs << "EndPosition," + endPos << endl;
		break;
	case Imgui::ParticleType::Lerp:
		type = "Lerp";
		break;
	default:
		break;
	}
}

void ParticleLoader::ParticleLoad(const char* f_fileName, ParticleData& data)
{
	ParticleData particleData = {};
	string pathName = f_fileName;
	string fullPath = "Resource/TextData/Particle/" + pathName + ".csv";

	ifstream file;
	file.open(fullPath);
	if (file.fail())
	{
		assert(0);
	}
	string keyType;
	while (getline(file, keyType))
	{
		istringstream line_stream(keyType);

		string key;
		getline(line_stream, key, ',');

		if (key == "Type")
		{
			getline(line_stream, key, ',');
			if (key == "Normal")
			{
				
			}
			else if (key == "Easeeing")
			{

			}
			else if (key == "Lerp")
			{

			}
		}
		else if (key == "Count")
		{
			line_stream >> particleData.count;
		}
		else if (key == "EmitterLife")
		{
			line_stream >> particleData.emitterLife;
		}
		else if (key == "ParticleLife")
		{
			line_stream >> particleData.particleLife;
		}
		else if (key == "SpornSpan")
		{
			line_stream >> particleData.spornSpan;
		}
		else if (key == "SpornArea")
		{
			line_stream >> particleData.spornArea[0];
			getline(line_stream, key, ',');
			line_stream >> particleData.spornArea[1];
			getline(line_stream, key, ',');
			line_stream >> particleData.spornArea[2];
		}
		else if (key == "StartSize")
		{

		}
		else if (key == "EndSize")
		{

		}

	}
}
