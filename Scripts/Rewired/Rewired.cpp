#include "Rewired.h"
#include "../BaseDirectX/dirent.h"
#include <fstream>
#include <string>
#include <sstream>

list<tuple<string, KeyCode, Sprite>> Rewired::KeyCodeString::keyboardKeys;
list<tuple<string, PadKeyCode, Sprite>> Rewired::KeyCodeString::padKeys;
list<Rewired::RewiredKeys*> Rewired::RewiredContainer::rewireds;
vector<std::string> Rewired::RewiredContainer::files;
list<Rewired::RewiredKeys> Rewired::RewiredContainer::rewiredsC;
//押されているかの判定
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
//押した瞬間の判定
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
//放された瞬間の判定
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
//キーの追加
void Rewired::RewiredKeys::AddKey(KeyCode key)
{
	if (keys.size() > 0)
	{
		for (auto itr = keys.begin(); itr != keys.end(); ++itr)
		{
			if (*itr == key)
			{
				//同じものを複数個登録しない
				return;
			}
		}
	}
	keys.push_back(key);
}
//キーの追加
void Rewired::RewiredKeys::AddKey(PadKeyCode key)
{
	if (padKeys.size() > 0)
	{
		for (auto itr = padKeys.begin(); itr != padKeys.end(); ++itr)
		{
			if (*itr == key)
			{
				//同じものを複数個登録しない
				return;
			}
		}
	}
	padKeys.push_back(key);
}
//キーの削除
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
//キーの削除
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
//キーの読み込み
void Rewired::RewiredKeys::LoadKey(const char* name, bool isAdd)
{
	//いったん全要素の削除
	if (keys.size() > 0)
	{
		keys.clear();
	}
	if (padKeys.size() > 0)
	{
		padKeys.clear();
	}
	//後にReloadするときなどに使う
	fileName = name;
	//ファイル検索
	string path = "Resource/TextData/Rewired/" + fileName + ".csv";
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
		getline(line_stream, key, ',');
		if (key == "KeyBoard")
		{
			for (auto itr = KeyCodeString::keyboardKeys.begin(); itr != KeyCodeString::keyboardKeys.end(); ++itr)
			{
				if (key + "," + std::get<0>(*itr) == line_stream.str())
				{
					keys.push_back(std::get<1>(*itr));
				}
			}
		}
		else if (key == "XboxPad")
		{
			for (auto itr = KeyCodeString::padKeys.begin(); itr != KeyCodeString::padKeys.end(); ++itr)
			{
				if (key + "," + std::get<0>(*itr) == line_stream.str())
				{
					padKeys.push_back(std::get<1>(*itr));
				}
			}
		}
	}
	if (!isAdd) return;
	//初回ロードの時はコンテナに登録する
	RewiredContainer::AddRewired(*this);
}
//キーの保存
void Rewired::RewiredKeys::SaveKey()
{
	//ファイルにキーを書き込む
	string saveFileName = "Resource/TextData/Rewired/" + fileName + ".csv";
	ofstream ofs(saveFileName);
	if (!ofs) return;
	for (auto itr = keys.begin(); itr != keys.end(); ++itr)
	{
		for (auto keyStringItr = Rewired::KeyCodeString::keyboardKeys.begin(); keyStringItr != Rewired::KeyCodeString::keyboardKeys.end(); ++keyStringItr)
		{
			if (std::get<1>(*keyStringItr) == *itr)
			{
				ofs << "KeyBoard," + std::get<0>(*keyStringItr) << std::endl;
			}
		}
	}
	for (auto itr = padKeys.begin(); itr != padKeys.end(); ++itr)
	{
		for (auto keyStringItr = Rewired::KeyCodeString::padKeys.begin(); keyStringItr != Rewired::KeyCodeString::padKeys.end(); ++keyStringItr)
		{
			if (std::get<1>(*keyStringItr) == *itr)
			{
				ofs << "XboxPad," + std::get<0>(*keyStringItr) << std::endl;
			}
		}
	}
}
//キーコードと文字列の準備
void Rewired::KeyCodeString::KeyCodeStringInit(BaseDirectX& baseDirectX)
{
	string path = "Resource/Image/Keys/";
	Sprite sA, sB, sC, sD, sE, sF, sG, sH, sI, sJ, sK, sL, sM, sN, sO, sP, sQ, sR, sS, sT, sU, sV,sW, sX, sY, sZ,
	s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sSpace, sLShift, sLCtrl, sUP, sRight, sLeft, sDown,
	padA, padB, padX, padY, padSelect, padPouse, padUp, padDown, padRight, padLeft;
	sA.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "A.png"), XMFLOAT3(0, 0, 0));
	sB.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "B.png"), XMFLOAT3(0, 0, 0));
	sC.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "C.png"), XMFLOAT3(0, 0, 0));
	sD.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "D.png"), XMFLOAT3(0, 0, 0));
	sE.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "E.png"), XMFLOAT3(0, 0, 0));
	sF.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "F.png"), XMFLOAT3(0, 0, 0));
	sG.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "G.png"), XMFLOAT3(0, 0, 0));
	sH.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "H.png"), XMFLOAT3(0, 0, 0));
	sI.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "I.png"), XMFLOAT3(0, 0, 0));
	sJ.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "J.png"), XMFLOAT3(0, 0, 0));
	sK.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "K.png"), XMFLOAT3(0, 0, 0));
	sL.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "L.png"), XMFLOAT3(0, 0, 0));
	sM.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "M.png"), XMFLOAT3(0, 0, 0));
	sN.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "N.png"), XMFLOAT3(0, 0, 0));
	sO.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "O.png"), XMFLOAT3(0, 0, 0));
	sP.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "P.png"), XMFLOAT3(0, 0, 0));
	sQ.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "Q.png"), XMFLOAT3(0, 0, 0));
	sR.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "R.png"), XMFLOAT3(0, 0, 0));
	sS.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "S.png"), XMFLOAT3(0, 0, 0));
	sT.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "T.png"), XMFLOAT3(0, 0, 0));
	sU.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "U.png"), XMFLOAT3(0, 0, 0));
	sV.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "V.png"), XMFLOAT3(0, 0, 0));
	sW.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "W.png"), XMFLOAT3(0, 0, 0));
	sX.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "X.png"), XMFLOAT3(0, 0, 0));
	sY.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "Y.png"), XMFLOAT3(0, 0, 0));
	sZ.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "Z.png"), XMFLOAT3(0, 0, 0));
	s0.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "0.png"), XMFLOAT3(0, 0, 0));
	s1.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "1.png"), XMFLOAT3(0, 0, 0));
	s2.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "2.png"), XMFLOAT3(0, 0, 0));
	s3.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "3.png"), XMFLOAT3(0, 0, 0));
	s4.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "4.png"), XMFLOAT3(0, 0, 0));
	s5.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "5.png"), XMFLOAT3(0, 0, 0));
	s6.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "6.png"), XMFLOAT3(0, 0, 0));
	s7.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "7.png"), XMFLOAT3(0, 0, 0));
	s8.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "8.png"), XMFLOAT3(0, 0, 0));
	s9.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "9.png"), XMFLOAT3(0, 0, 0));
	sSpace.CreateSprite( baseDirectX, ConvertStringToWchaer(path + "Space.png"),  XMFLOAT3(0, 0, 0));
	sLShift.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "LShift.png"), XMFLOAT3(0, 0, 0));
	sLCtrl.CreateSprite( baseDirectX, ConvertStringToWchaer(path + "LCtrl.png"),  XMFLOAT3(0, 0, 0));
	sUP.CreateSprite(    baseDirectX, ConvertStringToWchaer(path + "up.png"),     XMFLOAT3(0, 0, 0));
	sLeft.CreateSprite(  baseDirectX, ConvertStringToWchaer(path + "Left.png"),   XMFLOAT3(0, 0, 0));
	sRight.CreateSprite( baseDirectX, ConvertStringToWchaer(path + "Right.png"),  XMFLOAT3(0, 0, 0));
	sDown.CreateSprite(  baseDirectX, ConvertStringToWchaer(path + "Down.png"),   XMFLOAT3(0, 0, 0));

	padA.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "PadA.png"), XMFLOAT3(0, 0, 0));
	padB.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "PadB.png"), XMFLOAT3(0, 0, 0));
	padX.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "PadX.png"), XMFLOAT3(0, 0, 0));
	padY.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "PadY.png"), XMFLOAT3(0, 0, 0));
	padSelect.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "PadSelect.png"), XMFLOAT3(0, 0, 0));
	padUp.CreateSprite(    baseDirectX, ConvertStringToWchaer(path + "padUp.png"),     XMFLOAT3(0, 0, 0));
	padRight.CreateSprite( baseDirectX, ConvertStringToWchaer(path + "padRight.png"),  XMFLOAT3(0, 0, 0));
	padLeft.CreateSprite(  baseDirectX, ConvertStringToWchaer(path + "padLeft.png"),   XMFLOAT3(0, 0, 0));
	padDown.CreateSprite(  baseDirectX, ConvertStringToWchaer(path + "padDown.png"),   XMFLOAT3(0, 0, 0));


	tuple<string, KeyCode, Sprite> A = { "A", KeyCode::A , sA };
	tuple<string, KeyCode, Sprite> B = { "B", KeyCode::B , sB };
	tuple<string, KeyCode, Sprite> C = { "C", KeyCode::C , sC };
	tuple<string, KeyCode, Sprite> D = { "D", KeyCode::D , sD };
	tuple<string, KeyCode, Sprite> E = { "E", KeyCode::E , sE };
	tuple<string, KeyCode, Sprite> F = { "F", KeyCode::F , sF };
	tuple<string, KeyCode, Sprite> G = { "G", KeyCode::G , sG };
	tuple<string, KeyCode, Sprite> H = { "H", KeyCode::H , sH };
	tuple<string, KeyCode, Sprite> I = { "I", KeyCode::I , sI };
	tuple<string, KeyCode, Sprite> J = { "J", KeyCode::J , sJ };
	tuple<string, KeyCode, Sprite> K = { "K", KeyCode::K , sK };
	tuple<string, KeyCode, Sprite> L = { "L", KeyCode::L , sL };
	tuple<string, KeyCode, Sprite> M = { "M", KeyCode::M , sM };
	tuple<string, KeyCode, Sprite> N = { "N", KeyCode::N , sN };
	tuple<string, KeyCode, Sprite> O = { "O", KeyCode::O , sO };
	tuple<string, KeyCode, Sprite> P = { "P", KeyCode::P , sP };
	tuple<string, KeyCode, Sprite> Q = { "Q", KeyCode::Q , sQ };
	tuple<string, KeyCode, Sprite> R = { "R", KeyCode::R , sR };
	tuple<string, KeyCode, Sprite> S = { "S", KeyCode::S , sS };
	tuple<string, KeyCode, Sprite> T = { "T", KeyCode::T , sT };
	tuple<string, KeyCode, Sprite> U = { "U", KeyCode::U , sU };
	tuple<string, KeyCode, Sprite> V = { "V", KeyCode::V , sV };
	tuple<string, KeyCode, Sprite> W = { "W", KeyCode::W , sW };
	tuple<string, KeyCode, Sprite> X = { "X", KeyCode::X , sX };
	tuple<string, KeyCode, Sprite> Y = { "Y", KeyCode::Y , sY };
	tuple<string, KeyCode, Sprite> Z = { "Z", KeyCode::Z , sZ };
	tuple<string, KeyCode, Sprite> Num1 = { "1", KeyCode::Key1 , s0 };
	tuple<string, KeyCode, Sprite> Num2 = { "2", KeyCode::Key2 , s1 };
	tuple<string, KeyCode, Sprite> Num3 = { "3", KeyCode::Key3 , s2 };
	tuple<string, KeyCode, Sprite> Num4 = { "4", KeyCode::Key4 , s3 };
	tuple<string, KeyCode, Sprite> Num5 = { "5", KeyCode::Key5 , s4 };
	tuple<string, KeyCode, Sprite> Num6 = { "6", KeyCode::Key6 , s5 };
	tuple<string, KeyCode, Sprite> Num7 = { "7", KeyCode::Key7 , s6 };
	tuple<string, KeyCode, Sprite> Num8 = { "8", KeyCode::Key8 , s7 };
	tuple<string, KeyCode, Sprite> Num9 = { "9", KeyCode::Key9 , s8 };
	tuple<string, KeyCode, Sprite> Num0 = { "0", KeyCode::Key0 , s9 };
	tuple<string, KeyCode, Sprite> Tab =        { "Tab",        KeyCode::Tab        };
	tuple<string, KeyCode, Sprite> Space =      { "Space",      KeyCode::Space      ,sSpace  };
	tuple<string, KeyCode, Sprite> LShift =     { "LShift",     KeyCode::LShift     ,sLShift };
	tuple<string, KeyCode, Sprite> RShift =     { "RShift",     KeyCode::RShift     };
	tuple<string, KeyCode, Sprite> UpArrow =    { "UpArrow",    KeyCode::UpArrow    ,sUP};
	tuple<string, KeyCode, Sprite> DownArrow =  { "DownArrow",  KeyCode::DownArrow  ,sDown};
	tuple<string, KeyCode, Sprite> RightArrow = { "RightArrow", KeyCode::RightArrow ,sRight};
	tuple<string, KeyCode, Sprite> LeftArrow =  { "LeftArrow",  KeyCode::LeftArrow  ,sLeft};
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



	tuple<string, PadKeyCode, Sprite> PadUp =     { "PadUp",     PadKeyCode::UpButton     ,padUp};
	tuple<string, PadKeyCode, Sprite> PadDown =   { "PadDown",   PadKeyCode::DownButton   ,padDown};
	tuple<string, PadKeyCode, Sprite> PadLeft =   { "PadLeft",   PadKeyCode::LeftButton   ,padLeft};
	tuple<string, PadKeyCode, Sprite> PadRight =  { "PadRight",  PadKeyCode::RightButton  ,padRight};
	tuple<string, PadKeyCode, Sprite> PadA =      { "PadA",      PadKeyCode::Button01     ,padA };
	tuple<string, PadKeyCode, Sprite> PadB =      { "PadB",      PadKeyCode::Button02     ,padB };
	tuple<string, PadKeyCode, Sprite> PadX =      { "PadX",      PadKeyCode::Button03     ,padX };
	tuple<string, PadKeyCode, Sprite> PadY =      { "PadY",      PadKeyCode::Button04     ,padY };
	tuple<string, PadKeyCode, Sprite> PadLB =     { "PadLB",     PadKeyCode::ButtonLB     };
	tuple<string, PadKeyCode, Sprite> PadRB =     { "PadRB",     PadKeyCode::ButtonRB     };
	tuple<string, PadKeyCode, Sprite> PadSelect = { "PadSelect", PadKeyCode::ButtonSelect };
	tuple<string, PadKeyCode, Sprite> PadPouse =  { "PadPouse",  PadKeyCode::ButtonPouse  };
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
}
//実際に使っているもののアドレスを格納
void Rewired::RewiredContainer::AddRewired(RewiredKeys& rewired)
{
	rewireds.push_back(&rewired);
}
//Rewiredのロード
void Rewired::RewiredContainer::CreateRewired(string rewiredName)
{
	//ファイルを作る
	string saveFileName = "Resource/TextData/Rewired/" + rewiredName + ".csv";
	ofstream ofs(saveFileName);
	RewiredKeys key;
	key.LoadKey(rewiredName.c_str());
	for (auto itr = Rewired::RewiredContainer::rewiredsC.begin(); itr != Rewired::RewiredContainer::rewiredsC.end(); ++itr)
	{
		if (itr->GetFileName() == rewiredName)
		{
			return;
		}
	}
	Rewired::RewiredContainer::rewiredsC.push_back(key);
}
//Rewiredの再読み込み
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
//ImGuiに表示するための
//Rewiredフォルダ内の一括ロード
void Rewired::RewiredContainer::LoadAllRewired()
{
	for (auto itr = files.begin(); itr != files.end(); ++itr)
	{
		RewiredKeys key;
		key.LoadKey(itr->c_str(), false);
		rewiredsC.push_back(key);
	}
}
//Rewiredフォルダ内のファイルを取得し
//ファイル名だけを抽出
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
				//拡張子分削除する
				string extension = ".csv";
				size_t extensionSize = extension.size();
				loadFileName = loadFileName.substr(0, nameSize - extensionSize);
				files.push_back(loadFileName);
			}
		}
		closedir(dir);
	}
}
