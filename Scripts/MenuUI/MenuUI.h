#pragma once
#include "../Sprite/Sprite.h"
#include "../BaseDirectX/BaseDirectX.h"

class MenuUI
{
protected:
	//半透明黒
	Sprite pouseBack;
	//スクロールした値
	float pouseScroll;
	//ここまでスクロールできるように
	float maxpouseScroll;
	//一個の要素の大きさ
	float eachHeight;
	//これより大きいならスクロールできるようにする
	float showWindowHeight;

	list<pair<string, Sprite>> keySprites;

public:
	void LoadFile(BaseDirectX &baseDirectX);
	void LoadFile(BaseDirectX& baseDirectX, string name);
	void Init(float height = 5.0f);

	void DrawUI(BaseDirectX& baseDirectX);


};