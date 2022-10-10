#include "KeyLog.h"
#include "../imgui/ImguiControl.h"

UINT64 KeyLog::timer = 0;

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
		Rewired::KeyInfo<PadKeyCode> key
		;LogData<PadKeyCode>* log = new LogData<PadKeyCode>();
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

void KeyLog::Playback()
{

}

void KeyLog::SaveLog()
{
	//ƒtƒ@ƒCƒ‹‚ðì‚é
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
