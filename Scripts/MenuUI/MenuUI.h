#pragma once
#include "../Sprite/Sprite.h"
#include "../BaseDirectX/BaseDirectX.h"
#include "../Rewired/Rewired.h"

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

	bool mInputReception;

	Rewired::RewiredKeys *selectKey;
	Sprite mSMenu;
	Sprite mSFront;
	Sprite mSBack;
	Sprite mSRight;
	Sprite mSLeft;
	Sprite mSInputWait;
	void ShowKeySprite(BaseDirectX& baseDirectX, int index, int hiIndex);

public:
	void LoadFile(BaseDirectX& baseDirectX);
	void Init(float height = 5.0f);
	void Update();
	void DrawUI(BaseDirectX& baseDirectX);
};