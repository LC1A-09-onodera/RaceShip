#pragma once
#include "../BaseDirectX/Input.h"
#include "../Sprite/Sprite.h"

namespace Rewired
{
	template <typename T>
	struct KeyInfo
	{
		string keyName;
		T keyCode;
		Sprite icon;
	};
	class KeyCodeString
	{
	public:
		static void KeyCodeStringInit(BaseDirectX &baseDirectX);
		static bool GetAnyInput(KeyInfo<KeyCode> &key);
		static bool GetPadAnyInput(KeyInfo<PadKeyCode> &key );
		//Keyborad
		static list<KeyInfo<KeyCode>> mKeyboardKeys;
		//XBoxPad
		static list<KeyInfo<PadKeyCode>> mPadKeys;
	};
	class RewiredKeys
	{
		string fileName;
	public:
		string newKeyName;
		bool GetKey();
		bool GetKeyDown();
		bool GetKeyUp();
		void AddKey(KeyCode key);
		void AddKey(PadKeyCode key);
		void Subkey(KeyCode key);
		void SubKey(PadKeyCode key);
		void LoadKey(const char* name, bool isAdd = true);
		void SaveKey();
		string GetFileName(){ return fileName; }
		list<KeyCode> keys;
		list<PadKeyCode> padKeys;
	};
	class RewiredContainer
	{
	public:
		static vector<std::string> files;
		static list<RewiredKeys *> rewireds;
		static list<RewiredKeys> rewiredsC;
		static void AddRewired(RewiredKeys &rewired);
		//ImGuiでファイル作成する
		static void CreateRewired(string rewiredName, bool isAdd);
		//ファイル名は既存のを使うので初期化時には使えない
		static void ReloadRewired();
		static void LoadAllRewired();
		static void GetFilesName();
		static void AddKey(std::list<RewiredKeys>::iterator itr, int keyIndex);
		static void SubKey(std::list<RewiredKeys>::iterator itr, int keyIndex);
	};
}
