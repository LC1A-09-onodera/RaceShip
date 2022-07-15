#pragma once
#include "../Sprite/Sprite.h"
#include "../BaseDirectX/BaseDirectX.h"

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
	float eachHeight = 0.0f;
	//これより大きいならスクロールできるようにする
	float showWindowHeight = 0.0f;

	list<pair<string, Sprite>> keySprites;

public:
	void LoadFile(BaseDirectX &baseDirectX);
	void LoadFile(BaseDirectX& baseDirectX, string name);
	void Init(float height = 5.0f);

	void DrawUI(BaseDirectX& baseDirectX);


};