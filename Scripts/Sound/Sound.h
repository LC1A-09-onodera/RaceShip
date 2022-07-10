#pragma once
#include "../BaseDirectX/BaseDirectX.h"

struct ChunkHeader
{
	char id[4];
	int32_t size;
};

struct RiffHeader
{
	ChunkHeader chunk;
	char type[4];
};

struct FormatChunk
{
	ChunkHeader chunk;
	WAVEFORMATEX fmt;
};

struct SoundData
{
	~SoundData();
	WAVEFORMATEX wfex;
	std::shared_ptr<BYTE> pBuffer;
	unsigned int bufferSize;
	IXAudio2SourceVoice *pSourceVoice;
	XAUDIO2_BUFFER buf{};
};

class Sound
{
public:
	//音
	static ComPtr<IXAudio2> xAudio2;
	static IXAudio2MasteringVoice *masterVoice;
	//初期化
	static void CreateVoice(BaseDirectX& baseDirectX);
	static void Updete(float volume);
};

//サウンド読み込み
void SoundLoad(BaseDirectX& baseDirectX, const char *filename, SoundData &sound);
//サウンドの削除
void SoundUnload(SoundData *sounddata);
//サウンド再生
void SoundPlayerWave(BaseDirectX& baseDirectX, SoundData &soundData);
//サウンドを止める
void SoundStopWave(BaseDirectX& baseDirectX, SoundData &soundData);
//一度だけ鳴らす
void SoundPlayOnce(BaseDirectX& baseDirectX, SoundData &soundData);

void SoundPlayLoop(BaseDirectX& baseDirectX, SoundData &soundData);
//-10000~10000の間.0に向かって収束していく.0がミュート
void SoundVolume(SoundData &soundData, float volume);

class SoundControl
{
private:
	SoundControl();
	~SoundControl();
public:
	SoundControl(const SoundControl &obj) = delete;
	SoundControl &operator= (const SoundControl &obj) = delete;
	static SoundControl* GetSound();
};