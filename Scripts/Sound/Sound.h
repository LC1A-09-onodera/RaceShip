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
	~Sound()
	{
		delete(masterVoice);
	}
	//��
	static ComPtr<IXAudio2> xAudio2;
	static IXAudio2MasteringVoice *masterVoice;
	//������
	static void CreateVoice();
	static void Updete(float volume);
};

//�T�E���h�ǂݍ���
void SoundLoad( const char *filename, SoundData &sound);
//�T�E���h�̍폜
void SoundUnload(SoundData *sounddata);
//�T�E���h�Đ�
void SoundPlayerWave( SoundData &soundData);
//�T�E���h���~�߂�
void SoundStopWave( SoundData &soundData);
//��x�����炷
void SoundPlayOnce( SoundData &soundData);

void SoundPlayLoop( SoundData &soundData);
//-10000~10000�̊�.0�Ɍ������Ď������Ă���.0���~���[�g
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