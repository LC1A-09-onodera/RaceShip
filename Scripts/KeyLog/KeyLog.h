#pragma once
#include <string>
#include "../BaseDirectX/Input.h"
#include "../Rewired/Rewired.h"

using namespace std;

template<typename T>
class LogData
{
private:
	T _key;
	UINT64 _frame;
	//
	bool _isTrigger;
public:
	UINT64 GetFrame() { return this->_frame; };
	T GetKey() { return this->_key; };
	bool GetInputType() { return this->_isTrigger; };
	void SetFrame(UINT64& frame) { this->_frame = frame; };
	void SetKey(T& key) { this->_key = key; };
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
	string _fileName;
	list<LogData<KeyCode>> _keyLogs;
	list<LogData<PadKeyCode>> _padLogs;
public:
	static UINT64 timer;
	//ò^âÊ
	void Recording();
	//çƒê∂
	void Playback();

	void SaveLog();

	void SetFileName(string name);
};
