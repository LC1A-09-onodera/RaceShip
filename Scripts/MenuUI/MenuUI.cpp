#include "MenuUI.h"
#include "../Rewired/Rewired.h"

void MenuUI::LoadFile(BaseDirectX& baseDirectX)
{
	pouseBack.CreateSprite(baseDirectX, L"Resource/Image/PouseBackground.png", XMFLOAT3(0, 0, 0));
}

//void MenuUI::LoadFile(BaseDirectX& baseDirectX, string name)
//{
//	
//}
//
//void MenuUI::Init(float height)
//{
//	
//}

void MenuUI::DrawUI(BaseDirectX& baseDirectX)
{
	pouseBack.ChangeSize(baseDirectX, static_cast<float>(window_width), static_cast<float>(window_height));
	pouseBack.SpriteDraw(baseDirectX);
	int i = 0;
	int j = 0;
	const float uiSize = 32.0f;
	for (auto itr = Rewired::RewiredContainer::rewiredsC.begin(); itr != Rewired::RewiredContainer::rewiredsC.end(); ++itr)
	{
		for (auto keyItr = Rewired::KeyCodeString::mKeyboardKeys.begin(); keyItr != Rewired::KeyCodeString::mKeyboardKeys.end(); ++keyItr)
		{
			for (auto keysItr = itr->keys.begin(); keysItr != itr->keys.end(); ++keysItr)
			{
				if (keyItr->keyCode == *keysItr)
				{
					Sprite sp = keyItr->icon;
					sp.position.m128_f32[0] = i * uiSize;
					sp.position.m128_f32[1] = j * uiSize;
					sp.SpriteDraw(baseDirectX);
					i++;
				}
			}
		}
		for (auto padItr = Rewired::KeyCodeString::mPadKeys.begin(); padItr != Rewired::KeyCodeString::mPadKeys.end(); ++padItr)
		{
			for (auto padsItr = itr->padKeys.begin(); padsItr != itr->padKeys.end(); ++padsItr)
			{
				if (padItr->keyCode == *padsItr)
				{
					Sprite sp = padItr->icon;
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
