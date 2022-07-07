#pragma once
#include "../BaseDirectX/Input.h"

namespace Rewired
{
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
		string fileName;
	public:
		int combo;
		string newKeyName;
		char newKeyNameBuf[256];
		char newKeyTypeBuf[256];
		bool GetKey();
		bool GetKeyDown();
		bool GetKeyUp();
		void AddKey(KeyCode key);
		void AddKey(PadKeyCode key);
		void LoadKey(const char* name);
		string GetFileName(){ return fileName; }
		list<KeyCode> keys;
		list<PadKeyCode> padKeys;
	};

	class RewiredContainer
	{
	public:
		static list<RewiredKeys> rewireds;
		static void AddRewired(RewiredKeys &rewired);
		//ファイル名は既存のを使うので初期化時には使えない
		static void ReloadRewired();
	};
}
