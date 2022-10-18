#include "ParticleEdit.h"
#include <string>
#include <fstream>
#include <string>
#include <sstream>
#include <assert.h>
#include "../imgui/ImguiControl.h"

using namespace std;
ParticleData ImguiParticleDatas::particle[3];
int ImguiParticleDatas::activeIndex = 0;
void ParticleExport::CreatePartileFile(const char* f_fileName)
{
	ParticleData particle;

	particle.type = Imgui::particleType;
	particle.count = Imgui::particleCount;
	particle.emitterLife = Imgui::emitterLife;
	particle.particleLife = Imgui::particleLife;
	particle.spornSpan = Imgui::particleSpornSpan;
	particle.spornArea = { static_cast<float>(Imgui::particleSpornArea[0]), static_cast<float>(Imgui::particleSpornArea[1]) , static_cast<float>(Imgui::particleSpornArea[2]) };
	particle.startSize = Imgui::particleStartSize;
	particle.endsize = Imgui::particleEndSize;
	particle.speed = { Imgui::particleSpeed[0],Imgui::particleSpeed[1],Imgui::particleSpeed[2] };
	particle.speedDiff = { static_cast<float>(Imgui::particleSpeedDiff[0]), static_cast<float>(Imgui::particleSpeedDiff[1]) , static_cast<float>(Imgui::particleSpeedDiff[2]) };
	particle.acc = { Imgui::particleAcc[0], Imgui::particleAcc[1], Imgui::particleAcc[2] };
	
	json js;
	particle.ToJson(js);
	
	string particleFileName = f_fileName;
	//ÉtÉ@ÉCÉãÇçÏÇÈ
	string saveFileName = "Resource/TextData/Particle/" + particleFileName + ".json";
	ofstream ofs(saveFileName);
	if (!ofs) return;
	ofs << js.dump();
}

void ParticleLoader::ParticleLoad(const char* f_fileName, ParticleData& data)
{
	string pathName = f_fileName;
	string fullPath = "Resource/TextData/Particle/" + pathName + ".json";

	ifstream file;
	file.open(fullPath);
	if (file.fail())
	{
		assert(0);
	}
	string keyType;
	string allFileData = "";
	json parData;
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
