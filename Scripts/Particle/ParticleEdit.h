#include <array>
#pragma warning(push)
#pragma warning(disable:4996)
#pragma warning(disable:26800)
#include "../single_include/nlohmann/json.hpp"
#pragma warning(pop)

using json = nlohmann::json;

class ParticleData
{
public:
	enum ParticleType
	{
		Normal,
		Easeeing,
		Lerp,
	};
	int type;
	int count;
	int emitterLife;
	int particleLife;
	int spornSpan;
	std::array<float, 3> spornArea;
	float startSize;
	float endsize;
	float nowSize;
	std::array<float, 3> speed;
	std::array<float, 3> speedDiff;
	std::array<float, 3> acc;
	std::array<float, 3> startPosition;
	std::array<float, 3> endPosition;
	std::array<float, 3> position;
	int nowLife;
	int particleSpanTimer = 0;

	void ToJson(json& j);
	void FromJson(json& j);
};
class ParticleLoader
{
public:
	static void ParticleLoad(const char* f_fileName, ParticleData& data);
};

class ParticleExport
{
public:
	static void CreatePartileFile(const char* f_fileName);
};

class ImguiParticleDatas
{
public:
	static ParticleData particle[3];
	static int activeIndex;
};