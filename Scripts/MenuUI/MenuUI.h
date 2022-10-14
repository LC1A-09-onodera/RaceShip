#pragma once
#include "../Sprite/Sprite.h"
#include "../BaseDirectX/BaseDirectX.h"
#include "../Tools/Rewired/Rewired.h"

class MenuUI
{
protected:
	//半透明黒
	Sprite pouseBack;
	//スクロールした値
	float pouseScroll = 0.0f;
	//ここまでスクロールできるように
	float maxpouseScroll = 0.0f;
	//一個の要素の大きさ
	float eachHeight = 32.0f;
	//これより大きいならスクロールできるようにする
	float showWindowHeight = 0.0f;

	list<pair<string, Sprite>> keySprites;

	bool mInputReception = false;
	//既に使われているキーが押されたとき
	bool mIsAlreadyInUse = false;
	int alreadyTimer = 0;
	const int MaxAlreadyTime = 180;

	Rewired::RewiredKeys *selectKey = nullptr;
	Sprite mSMenu;
	Sprite mSFront;
	Sprite mSBack;
	Sprite mSRight;
	Sprite mSLeft;
	Sprite mSInputWait;
	Sprite mSAlreadyUse;
	void ShowKeySprite(BaseDirectX& baseDirectX, int index, int hiIndex);

public:
	void LoadFile(BaseDirectX& baseDirectX);
	void Init(float height = 5.0f);
	void Update();
	void DrawUI(BaseDirectX& baseDirectX);
};