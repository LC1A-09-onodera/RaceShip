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
	int j = 0;
	const float uiSize = 32.0f;
	for (auto itr = Rewired::RewiredContainer::rewiredsC.begin(); itr != Rewired::RewiredContainer::rewiredsC.end(); ++itr)
	{
		for (auto keyItr = Rewired::KeyCodeString::keyboardKeys.begin(); keyItr != Rewired::KeyCodeString::keyboardKeys.end(); ++keyItr)
		{
			for (auto keysItr = itr->keys.begin(); keysItr != itr->keys.end(); ++keysItr)
			{
				if (std::get<1>(*keyItr) == *keysItr)
				{
					Sprite sp = std::get<2>(*keyItr);
					sp.position.m128_f32[0] = i * uiSize;
					sp.position.m128_f32[1] = j * uiSize;
					sp.SpriteDraw(baseDirectX);
					i++;
				}
			}
			
		}
		for (auto padItr = Rewired::KeyCodeString::padKeys.begin(); padItr != Rewired::KeyCodeString::padKeys.end(); ++padItr)
		{
			for (auto padsItr = itr->padKeys.begin(); padsItr != itr->padKeys.end(); ++padsItr)
			{
				if (std::get<1>(*padItr) == *padsItr)
				{
					Sprite sp = std::get<2>(*padItr);
					sp.position.m128_f32[0] = i * uiSize;
					sp.position.m128_f32[1] = j * uiSize;
					sp.SpriteDraw(baseDirectX);
					i++;
				}
			}
		}
		i = 0;
		j++;
	}
}
