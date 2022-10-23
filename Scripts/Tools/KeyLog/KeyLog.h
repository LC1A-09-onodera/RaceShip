#pragma once
#include <string>
#include "../../BaseDirectX/Input.h"
#include "../../BaseDirectX/BaseDirectX.h"
#include "../Rewired/Rewired.h"

using namespace std;

template<typename T>
class LogData
{
private:
	T _data;
	UINT64 _frame;
	//
	bool _isTrigger;
public:
	UINT64 GetFrame() { return this->_frame; };
	T GetKey() { return this->_data; };
	bool GetInputType() { return this->_isTrigger; };
	void SetFrame(UINT64& frame) { this->_frame = frame; };
	void SetKey(T& key) { this->_data = key; };
	void SetInputType(bool isTrigger) { this->_isTrigger = isTrigger; };
	void Init(T& key, UINT64& frame, bool isTriggrt)
	{
		SetKey(key);
		SetFrame(frame);
		SetInputType(isTriggrt);
	};
};

class KeyLog
{
private:
	static string _fileName;
	static list<LogData<KeyCode>> _keyLogs;
	static list<LogData<PadKeyCode>> _padLogs;

	static list<LogData<KeyCode>> loadKeyList;

	static list<KeyCode> activeKeyList;
	static list<list<KeyCode>::iterator> deleteKeyList;
public:
	static UINT64 timer;
	static UINT64 playBackTimer;
	//ò^âÊ
	static void Recording();
	static void RecordingInit();
	//çƒê∂
	static void Playback();
	static void PlaybackInit();

	static void SaveLog();

	static void SetFileName(string name);
};

class PlayerPositionLog
{
private:
	static string _fileName;
	static list<LogData<XMFLOAT3>> _posLogs;
	static list<LogData<XMFLOAT3>> _loadList;
	static list<list<LogData<XMFLOAT3>>::iterator> _deleteList;
public:
	static UINT64 timer;
	static UINT64 playBackTimer;
	//ò^âÊ
	static void Recording(XMFLOAT3& pos);
	static void RecordingInit();
	//çƒê∂
	static void Playback(EachInfo &each);
	static void PlaybackInit();

	static void SaveLog();

	static void SetFileName(string name);
};