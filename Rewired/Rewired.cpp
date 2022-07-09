#include "Rewired.h"
#include "../BaseDirectX/dirent.h"
#include <fstream>
#include <string>
#include <sstream>

list<pair<string, KeyCode>> Rewired::KeyCodeString::keyboardKeys;
list<pair<string, PadKeyCode>> Rewired::KeyCodeString::padKeys;
list<Rewired::RewiredKeys*> Rewired::RewiredContainer::rewireds;
vector<std::string> Rewired::RewiredContainer::files;
list<Rewired::RewiredKeys> Rewired::RewiredContainer::rewiredsC;
bool Rewired::RewiredKeys::GetKey()
{
	for (auto keyItr = keys.begin(); keyItr != keys.end(); ++keyItr)
	{
		if (Input::Key(*keyItr))
		{
			return true;
		}
	}
	for (auto padItr = padKeys.begin(); padItr != padKeys.end(); ++padItr)
	{
		if (Input::directInput->IsButtonDown(*padItr))
		{
			return true;
		}
	}
	return false;
}

bool Rewired::RewiredKeys::GetKeyDown()
{
	for (auto keyItr = keys.begin(); keyItr != keys.end(); ++keyItr)
	{
		if (Input::KeyTrigger(*keyItr))
		{
			return true;
		}
	}
	for (auto padItr = padKeys.begin(); padItr != padKeys.end(); ++padItr)
	{
		if (Input::directInput->IsButtonPush(*padItr))
		{
			return true;
		}
	}
	return false;
}

bool Rewired::RewiredKeys::GetKeyUp()
{
	for (auto keyItr = keys.begin(); keyItr != keys.end(); ++keyItr)
	{
		if (Input::KeyEnd(*keyItr))
		{
			return true;
		}
	}
	for (auto padItr = padKeys.begin(); padItr != padKeys.end(); ++padItr)
	{
		if (Input::directInput->IsButtonUp(*padItr))
		{
			return true;
		}
	}
	return false;
}

void Rewired::RewiredKeys::AddKey(KeyCode key)
{
	if (keys.size() > 0)
	{
		for (auto itr = keys.begin(); itr != keys.end(); ++itr)
		{
			if (*itr == key)
			{
				//ìØÇ∂Ç‡ÇÃÇï°êîå¬ìoò^ÇµÇ»Ç¢
				return;
			}
		}
	}
	keys.push_back(key);
}

void Rewired::RewiredKeys::AddKey(PadKeyCode key)
{
	if (padKeys.size() > 0)
	{
		for (auto itr = padKeys.begin(); itr != padKeys.end(); ++itr)
		{
			if (*itr == key)
			{
				//ìØÇ∂Ç‡ÇÃÇï°êîå¬ìoò^ÇµÇ»Ç¢
				return;
			}
		}
	}
	padKeys.push_back(key);
}

void Rewired::RewiredKeys::Subkey(KeyCode key)
{
	if (keys.size() <= 0)return;

	for (auto itr = keys.begin(); itr != keys.end(); ++itr)
	{
		if (*itr == key)
		{
			keys.erase(itr);
			return;
		}
	}
}

void Rewired::RewiredKeys::SubKey(PadKeyCode key)
{
	if (padKeys.size() <= 0)return;

	for (auto itr = padKeys.begin(); itr != padKeys.end(); ++itr)
	{
		if (*itr == key)
		{
			padKeys.erase(itr);
			return;
		}
	}
}

void Rewired::RewiredKeys::LoadKey(const char* name, bool isAdd)
{
	if (keys.size() > 0)
	{
		keys.clear();
	}
	if (padKeys.size() > 0)
	{
		padKeys.clear();
	}
	fileName = name;
	string path = "Resource/TextData/Rewired/" + fileName + ".txt";
	ifstream file;
	file.open(path);
	if (file.fail())
	{
		assert(0);
	}
	string keyType;
	while (getline(file, keyType))
	{
		istringstream line_stream(keyType);

		string key;
		getline(line_stream, key, ' ');
		if (key == "KeyBoard")
		{
			for (auto itr = KeyCodeString::keyboardKeys.begin(); itr != KeyCodeString::keyboardKeys.end(); ++itr)
			{
				if (key + " " + itr->first == line_stream.str())
				{
					keys.push_back(itr->second);
				}
			}
		}
		else if (key == "XboxPad")
		{
			for (auto itr = KeyCodeString::padKeys.begin(); itr != KeyCodeString::padKeys.end(); ++itr)
			{
				if (key + " " + itr->first == line_stream.str())
				{
					padKeys.push_back(itr->second);
				}
			}
		}
	}
	if (!isAdd) return;
	RewiredContainer::AddRewired(*this);
}

void Rewired::RewiredKeys::SaveKey()
{
	string saveFileName = "Resource/TextData/Rewired/" + fileName + ".txt";
	ofstream ofs(saveFileName);
	if (!ofs) return;
	for (auto itr = keys.begin(); itr != keys.end(); ++itr)
	{
		for (auto keyStringItr = Rewired::KeyCodeString::keyboardKeys.begin(); keyStringItr != Rewired::KeyCodeString::keyboardKeys.end(); ++keyStringItr)
		{
			if (keyStringItr->second == *itr)
			{
				ofs << "KeyBoard " + keyStringItr->first << std::endl;
			}
		}
	}
	for (auto itr = padKeys.begin(); itr != padKeys.end(); ++itr)
	{
		for (auto keyStringItr = Rewired::KeyCodeString::padKeys.begin(); keyStringItr != Rewired::KeyCodeString::padKeys.end(); ++keyStringItr)
		{
			if (keyStringItr->second == *itr)
			{
				ofs << "XboxPad " + keyStringItr->first << std::endl;
			}
		}
	}
}

void Rewired::KeyCodeString::KeyCodeStringInit()
{
	pair<string, KeyCode> A = { "A", KeyCode::A };
	pair<string, KeyCode> B = { "B", KeyCode::B };
	pair<string, KeyCode> C = { "C", KeyCode::C };
	pair<string, KeyCode> D = { "D", KeyCode::D };
	pair<string, KeyCode> E = { "E", KeyCode::E };
	pair<string, KeyCode> F = { "F", KeyCode::F };
	pair<string, KeyCode> G = { "G", KeyCode::G };
	pair<string, KeyCode> H = { "H", KeyCode::H };
	pair<string, KeyCode> I = { "I", KeyCode::I };
	pair<string, KeyCode> J = { "J", KeyCode::J };
	pair<string, KeyCode> K = { "K", KeyCode::K };
	pair<string, KeyCode> L = { "L", KeyCode::L };
	pair<string, KeyCode> M = { "M", KeyCode::M };
	pair<string, KeyCode> N = { "N", KeyCode::N };
	pair<string, KeyCode> O = { "O", KeyCode::O };
	pair<string, KeyCode> P = { "P", KeyCode::P };
	pair<string, KeyCode> Q = { "Q", KeyCode::Q };
	pair<string, KeyCode> R = { "R", KeyCode::R };
	pair<string, KeyCode> S = { "S", KeyCode::S };
	pair<string, KeyCode> T = { "T", KeyCode::T };
	pair<string, KeyCode> U = { "U", KeyCode::U };
	pair<string, KeyCode> V = { "V", KeyCode::V };
	pair<string, KeyCode> W = { "W", KeyCode::W };
	pair<string, KeyCode> X = { "X", KeyCode::X };
	pair<string, KeyCode> Y = { "Y", KeyCode::Y };
	pair<string, KeyCode> Z = { "Z", KeyCode::Z };
	pair<string, KeyCode> Num1 = { "1", KeyCode::Key1 };
	pair<string, KeyCode> Num2 = { "2", KeyCode::Key2 };
	pair<string, KeyCode> Num3 = { "3", KeyCode::Key3 };
	pair<string, KeyCode> Num4 = { "4", KeyCode::Key4 };
	pair<string, KeyCode> Num5 = { "5", KeyCode::Key5 };
	pair<string, KeyCode> Num6 = { "6", KeyCode::Key6 };
	pair<string, KeyCode> Num7 = { "7", KeyCode::Key7 };
	pair<string, KeyCode> Num8 = { "8", KeyCode::Key8 };
	pair<string, KeyCode> Num9 = { "9", KeyCode::Key9 };
	pair<string, KeyCode> Num0 = { "0", KeyCode::Key0 };
	pair<string, KeyCode> Tab = { "Tab",        KeyCode::Tab };
	pair<string, KeyCode> Space = { "Space",      KeyCode::Space };
	pair<string, KeyCode> LShift = { "LShift",     KeyCode::LShift };
	pair<string, KeyCode> RShift = { "RShift",     KeyCode::RShift };
	pair<string, KeyCode> UpArrow = { "UpArrow",    KeyCode::UpArrow };
	pair<string, KeyCode> DownArrow = { "DownArrow",  KeyCode::DownArrow };
	pair<string, KeyCode> RightArrow = { "RightArrow", KeyCode::RightArrow };
	pair<string, KeyCode> LeftArrow = { "LeftArrow",  KeyCode::LeftArrow };
	keyboardKeys.push_back(A);
	keyboardKeys.push_back(B);
	keyboardKeys.push_back(C);
	keyboardKeys.push_back(D);
	keyboardKeys.push_back(E);
	keyboardKeys.push_back(F);
	keyboardKeys.push_back(G);
	keyboardKeys.push_back(H);
	keyboardKeys.push_back(I);
	keyboardKeys.push_back(J);
	keyboardKeys.push_back(K);
	keyboardKeys.push_back(L);
	keyboardKeys.push_back(M);
	keyboardKeys.push_back(N);
	keyboardKeys.push_back(O);
	keyboardKeys.push_back(P);
	keyboardKeys.push_back(Q);
	keyboardKeys.push_back(R);
	keyboardKeys.push_back(S);
	keyboardKeys.push_back(T);
	keyboardKeys.push_back(U);
	keyboardKeys.push_back(V);
	keyboardKeys.push_back(W);
	keyboardKeys.push_back(X);
	keyboardKeys.push_back(Y);
	keyboardKeys.push_back(Z);
	keyboardKeys.push_back(Num1);
	keyboardKeys.push_back(Num2);
	keyboardKeys.push_back(Num3);
	keyboardKeys.push_back(Num4);
	keyboardKeys.push_back(Num5);
	keyboardKeys.push_back(Num6);
	keyboardKeys.push_back(Num7);
	keyboardKeys.push_back(Num8);
	keyboardKeys.push_back(Num9);
	keyboardKeys.push_back(Num0);
	keyboardKeys.push_back(Tab);
	keyboardKeys.push_back(Space);
	keyboardKeys.push_back(LShift);
	keyboardKeys.push_back(RShift);
	keyboardKeys.push_back(UpArrow);
	keyboardKeys.push_back(DownArrow);
	keyboardKeys.push_back(RightArrow);
	keyboardKeys.push_back(LeftArrow);

	pair<string, PadKeyCode> PadUp = { "PadUp", PadKeyCode::UpButton };
	pair<string, PadKeyCode> PadDown = { "PadDown", PadKeyCode::DownButton };
	pair<string, PadKeyCode> PadLeft = { "PadLeft", PadKeyCode::LeftButton };
	pair<string, PadKeyCode> PadRight = { "PadRight", PadKeyCode::RightButton };
	pair<string, PadKeyCode> PadA = { "PadA", PadKeyCode::Button01 };
	pair<string, PadKeyCode> PadB = { "PadB", PadKeyCode::Button02 };
	pair<string, PadKeyCode> PadX = { "PadX", PadKeyCode::Button03 };
	pair<string, PadKeyCode> PadY = { "PadY", PadKeyCode::Button04 };
	pair<string, PadKeyCode> PadLB = { "PadLB", PadKeyCode::ButtonLB };
	pair<string, PadKeyCode> PadRB = { "PadRB", PadKeyCode::ButtonRB };
	pair<string, PadKeyCode> PadSelect = { "PadSelect", PadKeyCode::ButtonSelect };
	pair<string, PadKeyCode> PadPouse = { "PadPouse" , PadKeyCode::ButtonPouse };
	pair<string, PadKeyCode> Pad09 = { "Pad09", PadKeyCode::Button09 };
	pair<string, PadKeyCode> Pad10 = { "Pad10", PadKeyCode::Button10 };
	padKeys.push_back(PadUp);
	padKeys.push_back(PadDown);
	padKeys.push_back(PadLeft);
	padKeys.push_back(PadRight);
	padKeys.push_back(PadA);
	padKeys.push_back(PadB);
	padKeys.push_back(PadX);
	padKeys.push_back(PadY);
	padKeys.push_back(PadLB);
	padKeys.push_back(PadRB);
	padKeys.push_back(PadSelect);
	padKeys.push_back(PadPouse);
	padKeys.push_back(Pad09);
	padKeys.push_back(Pad10);
}

void Rewired::RewiredContainer::AddRewired(RewiredKeys& rewired)
{
	rewireds.push_back(&rewired);
}

void Rewired::RewiredContainer::CreateRewired(string rewiredName)
{
	//ÉtÉ@ÉCÉãÇçÏÇÈ
	string saveFileName = "Resource/TextData/Rewired/" + rewiredName + ".txt";
	ofstream ofs(saveFileName);
	RewiredKeys key;
	key.LoadKey(rewiredName.c_str());
}

void Rewired::RewiredContainer::ReloadRewired()
{
	for (auto itr = rewireds.begin(); itr != rewireds.end(); ++itr)
	{
		(*itr)->LoadKey((*itr)->GetFileName().c_str(), false);
	}
	for (auto itr = rewiredsC.begin(); itr != rewiredsC.end(); ++itr)
	{
		itr->LoadKey(itr->GetFileName().c_str(), false);
	}
}

void Rewired::RewiredContainer::LoadAllRewired()
{
	for (auto itr = files.begin(); itr != files.end(); ++itr)
	{
		RewiredKeys key;
		key.LoadKey(itr->c_str(), false);
		rewiredsC.push_back(key);
	}
}

void Rewired::RewiredContainer::GetFilesName()
{
	DIR* dir;
	dirent* diread;
	string path = "Resource/TextData/Rewired/";
	if ((dir = opendir(path.c_str())) != nullptr)
	{
		while ((diread = readdir(dir)) != nullptr)
		{
			if (diread->d_name[0] != '.')
			{
				string loadFileName = diread->d_name;
				size_t nameSize = loadFileName.size();
				//ägí£éqï™çÌèúÇ∑ÇÈ
				string extension = ".txt";
				size_t extensionSize = extension.size();
				loadFileName = loadFileName.substr(0, nameSize - extensionSize);
				files.push_back(loadFileName);
			}
		}
		closedir(dir);
	}
	else
	{
		perror("opendir");
		return;
	}
	return;
}
