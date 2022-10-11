#include <array>

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
};

class ParticleLoader
{
public:
	static void ParticleLoad(const char* f_fileName, ParticleData &data);
};

class ParticleExport
{
public:
	static void CreatePartileFile(const char* f_fileName);
};