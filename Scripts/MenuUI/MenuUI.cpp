#include "MenuUI.h"
#include "../Rewired/Rewired.h"

void MenuUI::LoadFile(BaseDirectX& baseDirectX)
{

}

void MenuUI::LoadFile(BaseDirectX& baseDirectX, string name)
{
	
}

void MenuUI::Init(float height)
{
}

void MenuUI::DrawUI(BaseDirectX& baseDirectX)
{
	int i = 0;
	const float uiSize = 32.0f;
	for (auto itr = Rewired::KeyCodeString::keyboardKeys.begin(); itr != Rewired::KeyCodeString::keyboardKeys.end(); ++itr)
	{
		Sprite sprite;
		sprite = std::get<2>(*itr);
		sprite.position.m128_f32[0] = i * uiSize;
		sprite.SpriteDraw(baseDirectX);
		i++;
	}
	i = 0;
	for (auto itr = Rewired::KeyCodeString::padKeys.begin(); itr != Rewired::KeyCodeString::padKeys.end(); ++itr)
	{
		Sprite sprite;
		sprite = std::get<2>(*itr);
		sprite.position.m128_f32[0] = i * uiSize;
		sprite.position.m128_f32[1] = uiSize;
		sprite.SpriteDraw(baseDirectX);
		i++;
	}
}
