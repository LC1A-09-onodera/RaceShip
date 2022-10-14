#include "Rewired.h"
#include "../../BaseDirectX/dirent.h"
#include <fstream>
#include <string>
#include <sstream>

list<Rewired::KeyInfo<KeyCode>> Rewired::KeyCodeString::mKeyboardKeys;
list<Rewired::KeyInfo<PadKeyCode>> Rewired::KeyCodeString::mPadKeys;
list<Rewired::RewiredKeys*> Rewired::RewiredContainer::rewireds;
vector<std::string> Rewired::RewiredContainer::files;
list<Rewired::RewiredKeys> Rewired::RewiredContainer::rewiredsC;
//������Ă��邩�̔���
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
//�������u�Ԃ̔���
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
//�����ꂽ�u�Ԃ̔���
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
//�L�[�̒ǉ�
void Rewired::RewiredKeys::AddKey(KeyCode key)
{
	if (keys.size() > 0)
	{
		for (auto itr = keys.begin(); itr != keys.end(); ++itr)
		{
			if (*itr == key)
			{
				//�������̂𕡐��o�^���Ȃ�
				return;
			}
		}
	}
	keys.push_back(key);
}
//�L�[�̒ǉ�
void Rewired::RewiredKeys::AddKey(PadKeyCode key)
{
	if (padKeys.size() > 0)
	{
		for (auto itr = padKeys.begin(); itr != padKeys.end(); ++itr)
		{
			if (*itr == key)
			{
				//�������̂𕡐��o�^���Ȃ�
				return;
			}
		}
	}
	padKeys.push_back(key);
}
//�L�[�̍폜
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
//�L�[�̍폜
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
//�L�[�̓ǂݍ���
void Rewired::RewiredKeys::LoadKey(const char* name, bool isAdd)
{
	//��������S�v�f�̍폜
	if (keys.size() > 0)
	{
		keys.clear();
	}
	if (padKeys.size() > 0)
	{
		padKeys.clear();
	}
	//���Reload����Ƃ��ȂǂɎg��
	fileName = name;
	//�t�@�C������
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
			for (auto itr = KeyCodeString::mKeyboardKeys.begin(); itr != KeyCodeString::mKeyboardKeys.end(); ++itr)
			{
				if (key + "," + itr->keyName == line_stream.str())
				{
					keys.push_back(itr->keyCode);
				}
			}
		}
		else if (key == "XboxPad")
		{
			for (auto itr = KeyCodeString::mPadKeys.begin(); itr != KeyCodeString::mPadKeys.end(); ++itr)
			{
				if (key + "," + itr->keyName == line_stream.str())
				{
					padKeys.push_back(itr->keyCode);
				}
			}
		}
	}
	if (!isAdd) return;
	//���񃍁[�h�̎��̓R���e�i�ɓo�^����
	RewiredContainer::AddRewired(*this);
}
//�L�[�̕ۑ�
void Rewired::RewiredKeys::SaveKey()
{
	//�t�@�C���ɃL�[����������
	string saveFileName = "Resource/TextData/Rewired/" + fileName + ".csv";
	ofstream ofs(saveFileName);
	if (!ofs) return;
	for (auto itr = keys.begin(); itr != keys.end(); ++itr)
	{
		for (auto keyStringItr = Rewired::KeyCodeString::mKeyboardKeys.begin(); keyStringItr != Rewired::KeyCodeString::mKeyboardKeys.end(); ++keyStringItr)
		{
			if (keyStringItr->keyCode == *itr)
			{
				ofs << "KeyBoard," + keyStringItr->keyName << std::endl;
			}
		}
	}
	for (auto itr = padKeys.begin(); itr != padKeys.end(); ++itr)
	{
		for (auto keyStringItr = Rewired::KeyCodeString::mPadKeys.begin(); keyStringItr != Rewired::KeyCodeString::mPadKeys.end(); ++keyStringItr)
		{
			if (keyStringItr->keyCode == *itr)
			{
				ofs << "XboxPad," + keyStringItr->keyName << std::endl;
			}
		}
	}
}
//�L�[�R�[�h�ƕ�����̏���
void Rewired::KeyCodeString::KeyCodeStringInit(BaseDirectX& baseDirectX)
{
	string path = "Resource/Image/Keys/";
	Sprite sA, sB, sC, sD, sE, sF, sG, sH, sI, sJ, sK, sL, sM, sN, sO, sP, sQ, sR, sS, sT, sU, sV,sW, sX, sY, sZ,
	s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sSpace, sLShift, sLCtrl, sUP, sRight, sLeft, sDown, sTab, sRShift,
	padA, padB, padX, padY, padSelect, padPouse, padUp, padDown, padRight, padLeft, padRb, padLb;
	sA.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "A.png"), XMFLOAT3(0, 0, 0) );
	sB.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "B.png"), XMFLOAT3(0, 0, 0) );
	sC.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "C.png"), XMFLOAT3(0, 0, 0) );
	sD.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "D.png"), XMFLOAT3(0, 0, 0) );
	sE.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "E.png"), XMFLOAT3(0, 0, 0) );
	sF.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "F.png"), XMFLOAT3(0, 0, 0) );
	sG.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "G.png"), XMFLOAT3(0, 0, 0) );
	sH.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "H.png"), XMFLOAT3(0, 0, 0) );
	sI.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "I.png"), XMFLOAT3(0, 0, 0) );
	sJ.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "J.png"), XMFLOAT3(0, 0, 0) );
	sK.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "K.png"), XMFLOAT3(0, 0, 0) );
	sL.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "L.png"), XMFLOAT3(0, 0, 0) );
	sM.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "M.png"), XMFLOAT3(0, 0, 0) );
	sN.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "N.png"), XMFLOAT3(0, 0, 0) );
	sO.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "O.png"), XMFLOAT3(0, 0, 0) );
	sP.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "P.png"), XMFLOAT3(0, 0, 0) );
	sQ.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "Q.png"), XMFLOAT3(0, 0, 0) );
	sR.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "R.png"), XMFLOAT3(0, 0, 0) );
	sS.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "S.png"), XMFLOAT3(0, 0, 0) );
	sT.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "T.png"), XMFLOAT3(0, 0, 0) );
	sU.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "U.png"), XMFLOAT3(0, 0, 0) );
	sV.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "V.png"), XMFLOAT3(0, 0, 0) );
	sW.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "W.png"), XMFLOAT3(0, 0, 0) );
	sX.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "X.png"), XMFLOAT3(0, 0, 0) );
	sY.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "Y.png"), XMFLOAT3(0, 0, 0) );
	sZ.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "Z.png"), XMFLOAT3(0, 0, 0) );
	s0.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "0.png"), XMFLOAT3(0, 0, 0) );
	s1.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "1.png"), XMFLOAT3(0, 0, 0) );
	s2.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "2.png"), XMFLOAT3(0, 0, 0) );
	s3.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "3.png"), XMFLOAT3(0, 0, 0) );
	s4.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "4.png"), XMFLOAT3(0, 0, 0) );
	s5.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "5.png"), XMFLOAT3(0, 0, 0) );
	s6.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "6.png"), XMFLOAT3(0, 0, 0) );
	s7.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "7.png"), XMFLOAT3(0, 0, 0) );
	s8.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "8.png"), XMFLOAT3(0, 0, 0) );
	s9.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "9.png"), XMFLOAT3(0, 0, 0) );
	sSpace.CreateSprite( baseDirectX, ConvertStringToWchaer(path + "Space.png"),  XMFLOAT3(0, 0, 0));
	sLShift.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "LShift.png"), XMFLOAT3(0, 0, 0));
	sLCtrl.CreateSprite( baseDirectX, ConvertStringToWchaer(path + "LCtrl.png"),  XMFLOAT3(0, 0, 0));
	sUP.CreateSprite(    baseDirectX, ConvertStringToWchaer(path + "up.png"),     XMFLOAT3(0, 0, 0));
	sLeft.CreateSprite(  baseDirectX, ConvertStringToWchaer(path + "Left.png"),   XMFLOAT3(0, 0, 0));
	sRight.CreateSprite( baseDirectX, ConvertStringToWchaer(path + "Right.png"),  XMFLOAT3(0, 0, 0));
	sDown.CreateSprite(  baseDirectX, ConvertStringToWchaer(path + "Down.png"),   XMFLOAT3(0, 0, 0));
	sTab.CreateSprite(   baseDirectX, ConvertStringToWchaer(path + "Tab.png"),    XMFLOAT3(0, 0, 0));
	sRShift.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "RS.png"),     XMFLOAT3(0, 0, 0));

	padA.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "PadA.png"), XMFLOAT3(0, 0, 0));
	padB.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "PadB.png"), XMFLOAT3(0, 0, 0));
	padX.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "PadX.png"), XMFLOAT3(0, 0, 0));
	padY.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "PadY.png"), XMFLOAT3(0, 0, 0));
	padSelect.CreateSprite(baseDirectX, ConvertStringToWchaer(path + "PadSelect.png"), XMFLOAT3(0, 0, 0));
	padPouse.CreateSprite( baseDirectX, ConvertStringToWchaer(path + "PadPouse.png"), XMFLOAT3(0, 0, 0));
	padUp.CreateSprite(    baseDirectX, ConvertStringToWchaer(path + "padUp.png"), XMFLOAT3(0, 0, 0));
	padRight.CreateSprite( baseDirectX, ConvertStringToWchaer(path + "padRight.png"), XMFLOAT3(0, 0, 0));
	padLeft.CreateSprite(  baseDirectX, ConvertStringToWchaer(path + "padLeft.png"), XMFLOAT3(0, 0, 0));
	padDown.CreateSprite(  baseDirectX, ConvertStringToWchaer(path + "padDown.png"), XMFLOAT3(0, 0, 0));
	padLb.CreateSprite(    baseDirectX, ConvertStringToWchaer(path + "LB.png"), XMFLOAT3(0, 0, 0));
	padRb.CreateSprite(    baseDirectX, ConvertStringToWchaer(path + "RB.png"), XMFLOAT3(0, 0, 0));

	Rewired::KeyInfo<KeyCode> A = { "A", KeyCode::A , sA };
	Rewired::KeyInfo<KeyCode> B = { "B", KeyCode::B , sB };
	Rewired::KeyInfo<KeyCode> C = { "C", KeyCode::C , sC };
	Rewired::KeyInfo<KeyCode> D = { "D", KeyCode::D , sD };
	Rewired::KeyInfo<KeyCode> E = { "E", KeyCode::E , sE };
	Rewired::KeyInfo<KeyCode> F = { "F", KeyCode::F , sF };
	Rewired::KeyInfo<KeyCode> G = { "G", KeyCode::G , sG };
	Rewired::KeyInfo<KeyCode> H = { "H", KeyCode::H , sH };
	Rewired::KeyInfo<KeyCode> I = { "I", KeyCode::I , sI };
	Rewired::KeyInfo<KeyCode> J = { "J", KeyCode::J , sJ };
	Rewired::KeyInfo<KeyCode> K = { "K", KeyCode::K , sK };
	Rewired::KeyInfo<KeyCode> L = { "L", KeyCode::L , sL };
	Rewired::KeyInfo<KeyCode> M = { "M", KeyCode::M , sM };
	Rewired::KeyInfo<KeyCode> N = { "N", KeyCode::N , sN };
	Rewired::KeyInfo<KeyCode> O = { "O", KeyCode::O , sO };
	Rewired::KeyInfo<KeyCode> P = { "P", KeyCode::P , sP };
	Rewired::KeyInfo<KeyCode> Q = { "Q", KeyCode::Q , sQ };
	Rewired::KeyInfo<KeyCode> R = { "R", KeyCode::R , sR };
	Rewired::KeyInfo<KeyCode> S = { "S", KeyCode::S , sS };
	Rewired::KeyInfo<KeyCode> T = { "T", KeyCode::T , sT };
	Rewired::KeyInfo<KeyCode> U = { "U", KeyCode::U , sU };
	Rewired::KeyInfo<KeyCode> V = { "V", KeyCode::V , sV };
	Rewired::KeyInfo<KeyCode> W = { "W", KeyCode::W , sW };
	Rewired::KeyInfo<KeyCode> X = { "X", KeyCode::X , sX };
	Rewired::KeyInfo<KeyCode> Y = { "Y", KeyCode::Y , sY };
	Rewired::KeyInfo<KeyCode> Z = { "Z", KeyCode::Z , sZ };
	Rewired::KeyInfo<KeyCode> Num1 = { "1", KeyCode::Key1 , s1 };
	Rewired::KeyInfo<KeyCode> Num2 = { "2", KeyCode::Key2 , s2 };
	Rewired::KeyInfo<KeyCode> Num3 = { "3", KeyCode::Key3 , s3 };
	Rewired::KeyInfo<KeyCode> Num4 = { "4", KeyCode::Key4 , s4 };
	Rewired::KeyInfo<KeyCode> Num5 = { "5", KeyCode::Key5 , s5 };
	Rewired::KeyInfo<KeyCode> Num6 = { "6", KeyCode::Key6 , s6 };
	Rewired::KeyInfo<KeyCode> Num7 = { "7", KeyCode::Key7 , s7 };
	Rewired::KeyInfo<KeyCode> Num8 = { "8", KeyCode::Key8 , s8 };
	Rewired::KeyInfo<KeyCode> Num9 = { "9", KeyCode::Key9 , s9 };
	Rewired::KeyInfo<KeyCode> Num0 = { "0", KeyCode::Key0 , s0 };
	Rewired::KeyInfo<KeyCode> Tab =        { "Tab",        KeyCode::Tab        ,sTab };
	Rewired::KeyInfo<KeyCode> Space =      { "Space",      KeyCode::Space      ,sSpace  };
	Rewired::KeyInfo<KeyCode> LShift =     { "LShift",     KeyCode::LShift     ,sLShift };
	Rewired::KeyInfo<KeyCode> RShift =     { "RShift",     KeyCode::RShift     ,sRShift };
	Rewired::KeyInfo<KeyCode> UpArrow =    { "UpArrow",    KeyCode::UpArrow    ,sUP };
	Rewired::KeyInfo<KeyCode> DownArrow =  { "DownArrow",  KeyCode::DownArrow  ,sDown };
	Rewired::KeyInfo<KeyCode> RightArrow = { "RightArrow", KeyCode::RightArrow ,sRight };
	Rewired::KeyInfo<KeyCode> LeftArrow =  { "LeftArrow",  KeyCode::LeftArrow  ,sLeft };
	mKeyboardKeys.push_back(A);
	mKeyboardKeys.push_back(B);
	mKeyboardKeys.push_back(C);
	mKeyboardKeys.push_back(D);
	mKeyboardKeys.push_back(E);
	mKeyboardKeys.push_back(F);
	mKeyboardKeys.push_back(G);
	mKeyboardKeys.push_back(H);
	mKeyboardKeys.push_back(I);
	mKeyboardKeys.push_back(J);
	mKeyboardKeys.push_back(K);
	mKeyboardKeys.push_back(L);
	mKeyboardKeys.push_back(M);
	mKeyboardKeys.push_back(N);
	mKeyboardKeys.push_back(O);
	mKeyboardKeys.push_back(P);
	mKeyboardKeys.push_back(Q);
	mKeyboardKeys.push_back(R);
	mKeyboardKeys.push_back(S);
	mKeyboardKeys.push_back(T);
	mKeyboardKeys.push_back(U);
	mKeyboardKeys.push_back(V);
	mKeyboardKeys.push_back(W);
	mKeyboardKeys.push_back(X);
	mKeyboardKeys.push_back(Y);
	mKeyboardKeys.push_back(Z);
	mKeyboardKeys.push_back(Num1);
	mKeyboardKeys.push_back(Num2);
	mKeyboardKeys.push_back(Num3);
	mKeyboardKeys.push_back(Num4);
	mKeyboardKeys.push_back(Num5);
	mKeyboardKeys.push_back(Num6);
	mKeyboardKeys.push_back(Num7);
	mKeyboardKeys.push_back(Num8);
	mKeyboardKeys.push_back(Num9);
	mKeyboardKeys.push_back(Num0);
	mKeyboardKeys.push_back(Tab);
	mKeyboardKeys.push_back(Space);
	mKeyboardKeys.push_back(LShift);
	mKeyboardKeys.push_back(RShift);
	mKeyboardKeys.push_back(UpArrow);
	mKeyboardKeys.push_back(DownArrow);
	mKeyboardKeys.push_back(RightArrow);
	mKeyboardKeys.push_back(LeftArrow);

	Rewired::KeyInfo<PadKeyCode> PadUp =     { "PadUp",     PadKeyCode::UpButton     ,padUp };
	Rewired::KeyInfo<PadKeyCode> PadDown =   { "PadDown",   PadKeyCode::DownButton   ,padDown };
	Rewired::KeyInfo<PadKeyCode> PadLeft =   { "PadLeft",   PadKeyCode::LeftButton   ,padLeft };
	Rewired::KeyInfo<PadKeyCode> PadRight =  { "PadRight",  PadKeyCode::RightButton  ,padRight };
	Rewired::KeyInfo<PadKeyCode> PadA =      { "PadA",      PadKeyCode::Button01     ,padA };
	Rewired::KeyInfo<PadKeyCode> PadB =      { "PadB",      PadKeyCode::Button02     ,padB };
	Rewired::KeyInfo<PadKeyCode> PadX =      { "PadX",      PadKeyCode::Button03     ,padX };
	Rewired::KeyInfo<PadKeyCode> PadY =      { "PadY",      PadKeyCode::Button04     ,padY };
	Rewired::KeyInfo<PadKeyCode> PadLB =     { "PadLB",     PadKeyCode::ButtonLB     ,padLb };
	Rewired::KeyInfo<PadKeyCode> PadRB =     { "PadRB",     PadKeyCode::ButtonRB     ,padRb };
	Rewired::KeyInfo<PadKeyCode> PadSelect = { "PadSelect", PadKeyCode::ButtonSelect ,padSelect };
	Rewired::KeyInfo<PadKeyCode> PadPouse =  { "PadPouse",  PadKeyCode::ButtonPouse  ,padPouse };
	mPadKeys.push_back(PadUp);
	mPadKeys.push_back(PadDown);
	mPadKeys.push_back(PadLeft);
	mPadKeys.push_back(PadRight);
	mPadKeys.push_back(PadA);
	mPadKeys.push_back(PadB);
	mPadKeys.push_back(PadX);
	mPadKeys.push_back(PadY);
	mPadKeys.push_back(PadLB);
	mPadKeys.push_back(PadRB);
	mPadKeys.push_back(PadSelect);
	mPadKeys.push_back(PadPouse);
}

bool Rewired::KeyCodeString::GetAnyTriggerInput(KeyInfo<KeyCode> &key)
{
	for (auto itr = mKeyboardKeys.begin(); itr != mKeyboardKeys.end(); ++itr)
	{
		if (Input::KeyTrigger(itr->keyCode))
		{
			key = (*itr);
			return true;
		}
	}
	return false;
}

bool Rewired::KeyCodeString::GetAnyTriggerInput()
{
	for (auto itr = mKeyboardKeys.begin(); itr != mKeyboardKeys.end(); ++itr)
	{
		if (Input::KeyTrigger(itr->keyCode))
		{
			return true;
		}
	}
	return false;
}

bool Rewired::KeyCodeString::GetAnyReleaseInput(KeyInfo<KeyCode>& key)
{
	for (auto itr = mKeyboardKeys.begin(); itr != mKeyboardKeys.end(); ++itr)
	{
		if (Input::KeyEnd(itr->keyCode))
		{
			key = (*itr);
			return true;
		}
	}
	return false;
}

bool Rewired::KeyCodeString::GetAnyReleaseInput()
{
	for (auto itr = mKeyboardKeys.begin(); itr != mKeyboardKeys.end(); ++itr)
	{
		if (Input::KeyEnd(itr->keyCode))
		{
			return true;
		}
	}
	return false;
}

bool Rewired::KeyCodeString::GetPadAnyTriggerInput(KeyInfo<PadKeyCode>& key)
{
	for (auto itr = mPadKeys.begin(); itr != mPadKeys.end(); ++itr)
	{
		if (Input::directInput->IsButtonDown(itr->keyCode))
		{
			key = (*itr);
			return true;
		}
	}
	return false;
}

bool Rewired::KeyCodeString::GetPadAnyTriggerInput()
{
	for (auto itr = mPadKeys.begin(); itr != mPadKeys.end(); ++itr)
	{
		if (Input::directInput->IsButtonDown(itr->keyCode))
		{
			return true;
		}
	}
	return false;
}

bool Rewired::KeyCodeString::GetPadAnyReleaseInput(KeyInfo<PadKeyCode>& key)
{
	for (auto itr = mPadKeys.begin(); itr != mPadKeys.end(); ++itr)
	{
		if (Input::directInput->IsButtonUp(itr->keyCode))
		{
			key = (*itr);
			return true;
		}
	}
	return false;
}

bool Rewired::KeyCodeString::GetPadAnyReleaseInput()
{
	for (auto itr = mPadKeys.begin(); itr != mPadKeys.end(); ++itr)
	{
		if (Input::directInput->IsButtonDown(itr->keyCode))
		{
			return true;
		}
	}
	return false;
}

//���ۂɎg���Ă�����̂̃A�h���X���i�[
void Rewired::RewiredContainer::AddRewired(RewiredKeys& rewired)
{
	rewireds.push_back(&rewired);
}
//Rewired�̃��[�h
void Rewired::RewiredContainer::CreateRewired(string rewiredName, bool isAdd)
{
	//�t�@�C�������
	string saveFileName = "Resource/TextData/Rewired/" + rewiredName + ".csv";
	ofstream ofs(saveFileName);
	RewiredKeys key;
	key.LoadKey(rewiredName.c_str(), isAdd);
	for (auto itr = Rewired::RewiredContainer::rewiredsC.begin(); itr != Rewired::RewiredContainer::rewiredsC.end(); ++itr)
	{
		if (itr->GetFileName() == rewiredName)
		{
			return;
		}
	}
	Rewired::RewiredContainer::rewiredsC.push_back(key);
}
//Rewired�̍ēǂݍ���
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
//ImGui�ɕ\�����邽�߂�
//Rewired�t�H���_���̈ꊇ���[�h
void Rewired::RewiredContainer::LoadAllRewired()
{
	for (auto itr = files.begin(); itr != files.end(); ++itr)
	{
		RewiredKeys key;
		key.LoadKey(itr->c_str(), false);
		rewiredsC.push_back(key);
	}
}
//Rewired�t�H���_���̃t�@�C�����擾��
//�t�@�C���������𒊏o
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
				//�g���q���폜����
				string extension = ".csv";
				size_t extensionSize = extension.size();
				loadFileName = loadFileName.substr(0, nameSize - extensionSize);
				files.push_back(loadFileName);
			}
		}
		closedir(dir);
	}
}

void Rewired::RewiredContainer::AddKey(std::list<RewiredKeys>::iterator itr, int keyIndex)
{
	if (static_cast<int>(KeyCode::KeyCodeMax) - 1 > keyIndex)
	{
		auto keyStringItr = Rewired::KeyCodeString::mKeyboardKeys.begin();
		if (Rewired::KeyCodeString::mKeyboardKeys.size() > 0)
		{
			for (int i = 0; i < keyIndex; i++)
			{
				keyStringItr++;
			}
		}
		bool isA = false;
		//���Ɏg���Ă���L�[���ēo�^���Ȃ�
		for (auto keyContainarItr = Rewired::RewiredContainer::rewiredsC.begin(); keyContainarItr != Rewired::RewiredContainer::rewiredsC.end(); ++keyContainarItr)
		{
			for (auto kItr = keyContainarItr->keys.begin(); kItr != keyContainarItr->keys.end(); ++kItr)
			{
				if (*kItr == keyStringItr->keyCode)
				{
					isA = true;
				}
			}
		}
		if (!isA) itr->AddKey(keyStringItr->keyCode);
	}
	else
	{
		auto padStringItr = Rewired::KeyCodeString::mPadKeys.begin();
		if (Rewired::KeyCodeString::mPadKeys.size() > 0)
		{
			for (int j = 0; j < keyIndex - ((static_cast<int>(KeyCode::KeyCodeMax)) - 1); j++)
			{
				padStringItr++;
			}
		}
		//���Ɏg���Ă���L�[���ēo�^���Ȃ�
		bool isA = false;
		for (auto keyContainarItr = Rewired::RewiredContainer::rewiredsC.begin(); keyContainarItr != Rewired::RewiredContainer::rewiredsC.end(); ++keyContainarItr)
		{
			for (auto kItr = keyContainarItr->padKeys.begin(); kItr != keyContainarItr->padKeys.end(); ++kItr)
			{
				if (*kItr == padStringItr->keyCode)
				{
					isA = true;
				}
			}
		}
		if (!isA) itr->AddKey(padStringItr->keyCode);
	}
}

void Rewired::RewiredContainer::SubKey(std::list<RewiredKeys>::iterator itr, int keyIndex)
{
	if (static_cast<int>(KeyCode::KeyCodeMax) - 1 > keyIndex)
	{
		auto keyStringItr = Rewired::KeyCodeString::mKeyboardKeys.begin();
		if (Rewired::KeyCodeString::mKeyboardKeys.size() > 0)
		{
			for (int i = 0; i < keyIndex; i++)
			{
				keyStringItr++;
			}
		}
		itr->Subkey(keyStringItr->keyCode);
	}
	else
	{
		auto padStringItr = Rewired::KeyCodeString::mPadKeys.begin();
		if (Rewired::KeyCodeString::mPadKeys.size() > 0)
		{
			for (int j = 0; j < keyIndex - ((static_cast<int>(KeyCode::KeyCodeMax)) - 1); j++)
			{
				padStringItr++;
			}
		}
		itr->SubKey(padStringItr->keyCode);
	}
}
