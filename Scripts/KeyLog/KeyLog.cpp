#include "KeyLog.h"
#include "../imgui/ImguiControl.h"

UINT64 KeyLog::timer = 0;
string KeyLog::_fileName;
list<LogData<KeyCode>> KeyLog::_keyLogs;
list<LogData<PadKeyCode>> KeyLog::_padLogs;
UINT64 KeyLog::playBackTimer = 0;
list<LogData<KeyCode>> KeyLog::loadKeyList;
list<KeyCode> KeyLog::activeKeyList;
list<list<KeyCode>::iterator> KeyLog::deleteKeyList;
void KeyLog::Recording()
{
	timer++;
	if (Rewired::KeyCodeString::GetAnyTriggerInput() || Rewired::KeyCodeString::GetAnyReleaseInput())
	{
		Rewired::KeyInfo<KeyCode> key;
		LogData<KeyCode>* log = new LogData<KeyCode>();
		if (Rewired::KeyCodeString::GetAnyTriggerInput())
		{
			Rewired::KeyCodeString::GetAnyTriggerInput(key);
			log->Init(key.keyCode, timer, true);
			_keyLogs.push_back(*log);
		}
		if (Rewired::KeyCodeString::GetAnyReleaseInput())
		{
			Rewired::KeyCodeString::GetAnyReleaseInput(key);
			log->Init(key.keyCode, timer, false);
			_keyLogs.push_back(*log);
		}
	}
	if (Rewired::KeyCodeString::GetPadAnyReleaseInput() || Rewired::KeyCodeString::GetPadAnyReleaseInput())
	{
		Rewired::KeyInfo<PadKeyCode> key;
		LogData<PadKeyCode>* log = new LogData<PadKeyCode>();
		if (Rewired::KeyCodeString::GetPadAnyReleaseInput())
		{
			Rewired::KeyCodeString::GetPadAnyReleaseInput(key);
			log->Init(key.keyCode, timer, true);
			_padLogs.push_back(*log);
		}
		if (Rewired::KeyCodeString::GetPadAnyReleaseInput())
		{
			Rewired::KeyCodeString::GetPadAnyReleaseInput(key);
			log->Init(key.keyCode, timer, false);
			_padLogs.push_back(*log);
		}
	}
}

void KeyLog::RecordingInit()
{
	_keyLogs.clear();
	_padLogs.clear();
	timer = 0;
}

void KeyLog::Playback()
{
	if (Imgui::playBackFrame > playBackTimer)
	{
		playBackTimer++;
	}
	else
	{
		for (auto activeItr = activeKeyList.begin(); activeItr != activeKeyList.end(); ++activeItr)
		{
			deleteKeyList.push_back(activeItr);
		}
	}
	//activeKeyを実際の入力に適応させる
	for (auto loadItr = loadKeyList.begin(); loadItr != loadKeyList.end(); ++loadItr)
	{
		if (loadItr->GetFrame() == playBackTimer)
		{
			if (loadItr->GetInputType())
			{
				activeKeyList.push_back(loadItr->GetKey());
			}
			else
			{
				for (auto activeItr = activeKeyList.begin(); activeItr != activeKeyList.end(); ++activeItr)
				{
					if (*activeItr == loadItr->GetKey())
					{
						deleteKeyList.push_back(activeItr);
					}
				}
			}
		}
	}
	//Releaseされたキーの削除
	for (auto deleteItr = deleteKeyList.begin(); deleteItr != deleteKeyList.end(); ++deleteItr)
	{
		activeKeyList.remove(*(*deleteItr));
	}
	deleteKeyList.clear();
	//有効キーを入力状態にする
	for (auto activeItr = activeKeyList.begin(); activeItr != activeKeyList.end(); ++activeItr)
	{
		int keyNum = static_cast<int>(*activeItr);
		Input::keys[keyNum] = (BYTE)true;
	}
}

void KeyLog::PlaybackInit()
{
	loadKeyList.clear();
	playBackTimer = 0;
	//ファイル読み込み
	string pathName = _fileName;
	string fullPath = "Resource/TextData/Log/" + pathName + ".csv";

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
		//Frame
		LogData<KeyCode> log;
		getline(line_stream, key, ',');
		UINT64 frame = std::stoi(key);
		log.SetFrame(frame);
		//Key
		getline(line_stream, key, ',');
		getline(line_stream, key, ',');
		for (auto keyListItr = Rewired::KeyCodeString::mKeyboardKeys.begin(); keyListItr != Rewired::KeyCodeString::mKeyboardKeys.end(); ++keyListItr)
		{
			if (keyListItr->keyName == key)
			{
				log.SetKey(keyListItr->keyCode);
			}
		}
		//InputType
		getline(line_stream, key, ',');
		if (key == "Trigger")
		{
			log.SetInputType(true);
		}
		else
		{
			log.SetInputType(false);
		}
		loadKeyList.push_back(log);
	}
	int a = static_cast<int>(loadKeyList.size());
}

void KeyLog::SaveLog()
{
	//ファイルを作る
	string saveFileName = "Resource/TextData/Log/" + _fileName + ".csv";
	ofstream ofs(saveFileName);
	if (!ofs) return;
	auto keyItr = _keyLogs.begin();
	auto padItr = _padLogs.begin();
	while (keyItr != _keyLogs.end() || padItr != _padLogs.end())
	{
		if (keyItr != _keyLogs.end())
		{
			for (auto keyString = Rewired::KeyCodeString::mKeyboardKeys.begin(); keyString != Rewired::KeyCodeString::mKeyboardKeys.end(); ++keyString)
			{
				if (keyString->keyCode == keyItr->GetKey())
				{
					string inputType;
					if (keyItr->GetInputType())
					{
						inputType = "Trigger";
					}
					else
					{
						inputType = "Release";
					}
					ofs << "Frame," << keyItr->GetFrame() << "," << "Key," << keyString->keyName << "," << inputType << std::endl;
				}
			}
			keyItr++;
		}
		if (padItr != _padLogs.end())
		{
			for (auto padString = Rewired::KeyCodeString::mPadKeys.begin(); padString != Rewired::KeyCodeString::mPadKeys.end(); ++padString)
			{
				if (padString->keyCode == padItr->GetKey())
				{
					string inputType;
					if (keyItr->GetInputType())
					{
						inputType = "Trigger";
					}
					else
					{
						inputType = "Release";
					}
					ofs << "Frame," << keyItr->GetFrame() << "," << "Key," << padString->keyName << "," << inputType << std::endl;
				}
			}
			padItr++;
		}
	}
}

void KeyLog::SetFileName(string name)
{
	_fileName = name;
}
