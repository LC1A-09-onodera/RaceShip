#pragma once
#include "../BaseDirectX/Input.h"

namespace Rewired
{
	class InputKey
	{

	};

	class KeyCodeString
	{
	public:
		static void KeyCodeStringInit();
		//Keyborad
		static list<pair<string, KeyCode>> keyboardKeys;
		//XBoxPad
		static list<pair<string, PadKeyCode>> padKeys;
	};

	class RewiredKeys
	{
		list<KeyCode> keys;
		list<PadKeyCode> padKeys;
	public:
		bool GetKey();
		bool GetKeyDown();
		bool GetKeyUp();
		void AddKey(KeyCode key);

		void LoadKey(const char* path);
	};
}
