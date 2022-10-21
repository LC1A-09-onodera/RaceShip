#include "ParticleEdit.h"
#include <string>
#include <fstream>
#include <string>
#include <sstream>
#include <assert.h>
#include "../../imgui/ImguiControl.h"

using namespace std;

namespace Editors
{
	void ParticleExport::CreatePartileFile(const char* f_fileName)
	{
		ParticleData particle;
		particle.acc = { Imgui::GetInstance()->particleAcc[0], Imgui::GetInstance()->particleAcc[1], Imgui::GetInstance()->particleAcc[2] };
		particle.count = Imgui::GetInstance()->particleCount;
		particle.emitterLife = Imgui::GetInstance()->emitterLife;
		particle.endPosition = { Imgui::GetInstance()->particleEndPosition[0], Imgui::GetInstance()->particleEndPosition[1], Imgui::GetInstance()->particleEndPosition[2] };
		particle.endsize = Imgui::GetInstance()->particleEndSize;
		particle.nowLife = 0;
		particle.nowSize = Imgui::GetInstance()->particleStartSize;
		particle.particleLife = Imgui::GetInstance()->particleLife;
		//particle.particleSpanTimer = Imgui::GetInstance()->particleSpornSpan;
		particle.speed = { Imgui::GetInstance()->particleSpeed[0],Imgui::GetInstance()->particleSpeed[1],Imgui::GetInstance()->particleSpeed[2] };
		particle.speedDiff = { static_cast<float>(Imgui::GetInstance()->particleSpeedDiff[0]), static_cast<float>(Imgui::GetInstance()->particleSpeedDiff[1]) , static_cast<float>(Imgui::GetInstance()->particleSpeedDiff[2]) };
		particle.spornArea = { static_cast<float>(Imgui::GetInstance()->particleSpornArea[0]), static_cast<float>(Imgui::GetInstance()->particleSpornArea[1]) , static_cast<float>(Imgui::GetInstance()->particleSpornArea[2]) };
		particle.spornSpan = Imgui::GetInstance()->particleSpornSpan;
		particle.type = Imgui::GetInstance()->particleType;
		particle.startSize = Imgui::GetInstance()->particleStartSize;
		string allData = "";
		json js;
		particle.ToJson(js);
		allData = js.dump();

		string particleFileName = f_fileName;
		//ÉtÉ@ÉCÉãÇçÏÇÈ
		string saveFileName = "Resource/TextData/Particle/" + particleFileName + ".csv";
		ofstream ofs(saveFileName);
		if (!ofs) return;
		ofs << allData;
	}

	void ParticleLoader::ParticleLoad(const char* f_fileName, ParticleData& data)
	{
		string pathName = f_fileName;
		string fullPath = "Resource/TextData/Particle/" + pathName + ".csv";

		ifstream file;
		file.open(fullPath);
		if (file.fail())
		{
			assert(0);
		}
		string keyType;
		string allFileData = "";

		while (getline(file, keyType))
		{
			allFileData += keyType;
		}
		json parConv = json::parse(allFileData);
		data.FromJson(parConv);
		int a = 10;
	}

	void ParticleData::ToJson(json& j)
	{
		j = json{
			{"Type", type},
			{"Count",count},
			{"EmitterLife",emitterLife},
			{"ParticleLife",particleLife},
			{"SpornSpan",spornSpan},
			{"SpornArea",spornArea} ,
			{"StartSize",startSize},
			{"EndSize",endsize},
			{"Speed",speed},
			{"SpeedDiff",speedDiff},
			{"Acc" ,acc }
		};
	}

	void ParticleData::FromJson(json& j)
	{
		j.at("Type").get_to(type);
		j.at("Count").get_to(count);
		j.at("EmitterLife").get_to(emitterLife);
		j.at("ParticleLife").get_to(particleLife);
		j.at("SpornSpan").get_to(spornSpan);
		j.at("SpornArea").get_to(spornArea);
		j.at("StartSize").get_to(startSize);
		j.at("EndSize").get_to(endsize);
		j.at("Speed").get_to(speed);
		j.at("SpeedDiff").get_to(speedDiff);
		j.at("Acc").get_to(acc);
	}
}
