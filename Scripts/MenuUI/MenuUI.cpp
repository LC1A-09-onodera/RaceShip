#include "MenuUI.h"
#include "../Rewired/Rewired.h"

void MenuUI::ShowKeySprite(BaseDirectX& baseDirectX, int index, int hiIndex)
{
	int i = 0;
	int j = hiIndex;
	const float uiSize = 32.0f;
	const float actionUIWid = 128.0f;
	const float baseX = 550.0f;
	const float baseY = 300.0f;

	XMFLOAT3 mousePosi = { static_cast<float>(WindowsAPI::GetMousePos().x), static_cast<float>(WindowsAPI::GetMousePos().y), 0 };
	XMFLOAT3 leftUp = { baseX, baseY + (uiSize * hiIndex), 0 };
	if (CollisionRectangle2D(leftUp, actionUIWid, actionUIWid, mousePosi) && Input::MouseTrigger(MouseButton::LBUTTON))
	{
		mInputReception = true;
		auto itr = Rewired::RewiredContainer::rewiredsC.begin();
		for (int k = 0; k < index; k++)
		{
			itr++;
		}
		selectKey = &(*itr);
	}
	auto itr = Rewired::RewiredContainer::rewiredsC.begin();
	for (int k = 0; k < index; k++)
	{
		itr++;
	}
	for (auto keyItr = Rewired::KeyCodeString::mKeyboardKeys.begin(); keyItr != Rewired::KeyCodeString::mKeyboardKeys.end(); ++keyItr)
	{
		for (auto keysItr = itr->keys.begin(); keysItr != itr->keys.end(); ++keysItr)
		{
			if (keyItr->keyCode != *keysItr)continue;
			Sprite sp = keyItr->icon;
			sp.position.m128_f32[0] = i * uiSize + baseX + actionUIWid;
			sp.position.m128_f32[1] = j * uiSize + baseY;
			sp.SpriteDraw(baseDirectX);
			i++;
		}
	}
	for (auto padItr = Rewired::KeyCodeString::mPadKeys.begin(); padItr != Rewired::KeyCodeString::mPadKeys.end(); ++padItr)
	{
		for (auto padsItr = itr->padKeys.begin(); padsItr != itr->padKeys.end(); ++padsItr)
		{
			if (padItr->keyCode != *padsItr)continue;
			Sprite sp = padItr->icon;
			sp.position.m128_f32[0] = i * uiSize + baseX + actionUIWid;
			sp.position.m128_f32[1] = j * uiSize + baseY;
			sp.SpriteDraw(baseDirectX);
			i++;
		}
	}
}

void MenuUI::LoadFile(BaseDirectX& baseDirectX)
{
	pouseBack.CreateSprite(baseDirectX, L"Resource/Image/PouseBackground.png", XMFLOAT3(0, 0, 0));
	mSMenu.CreateSprite(baseDirectX, L"Resource/Image/Menu.png", XMFLOAT3(0, 0, 0));
	mSFront.CreateSprite(baseDirectX, L"Resource/Image/KeyForward.png", XMFLOAT3(0, 0, 0));
	mSBack.CreateSprite(baseDirectX, L"Resource/Image/KeyBackward.png", XMFLOAT3(0, 0, 0));
	mSRight.CreateSprite(baseDirectX, L"Resource/Image/KeyRight.png", XMFLOAT3(0, 0, 0));
	mSLeft.CreateSprite(baseDirectX, L"Resource/Image/KeyLeft.png", XMFLOAT3(0, 0, 0));
	mSInputWait.CreateSprite(baseDirectX, L"Resource/Image/InputWait.png", XMFLOAT3(0, 0, 0));
	mSAlreadyUse.CreateSprite(baseDirectX, L"Resource/Image/AlreadyUse.png", XMFLOAT3(0, 0, 0));
}

void MenuUI::Update()
{
	if (mInputReception)
	{
		Rewired::KeyInfo<KeyCode> keyInfo;
		Rewired::KeyInfo<PadKeyCode> padInfo;
		if (Rewired::KeyCodeString::GetAnyTriggerInput(keyInfo))
		{
			auto itr = selectKey->keys.begin();
			for (auto Citr = Rewired::RewiredContainer::rewiredsC.begin(); Citr != Rewired::RewiredContainer::rewiredsC.end(); ++Citr)
			{
				auto keysItr = Citr->keys.begin();
				for (int i = 0; i < Citr->keys.size(); i++)
				{
					if (*keysItr == keyInfo.keyCode)
					{
						mInputReception = false;
						mIsAlreadyInUse = true;
						selectKey->SaveKey();
						Rewired::RewiredContainer::ReloadRewired();
						return;
					}
					keysItr++;
				}
			}
			*itr = keyInfo.keyCode;
			mInputReception = false;
			selectKey->SaveKey();
			Rewired::RewiredContainer::ReloadRewired();
		}
		else if (Rewired::KeyCodeString::GetPadAnyTriggerInput(padInfo))
		{
			mInputReception = false;
		}
	}
	else
	{
		if (Input::KeyTrigger(DIK_1))
		{
			mInputReception = true;
			selectKey = (&(*Rewired::RewiredContainer::rewiredsC.begin()));
		}
	}
}

void MenuUI::DrawUI(BaseDirectX& baseDirectX)
{
	Update();
	pouseBack.ChangeSize(baseDirectX, static_cast<float>(window_width), static_cast<float>(window_height));
	pouseBack.SpriteDraw(baseDirectX);

	mSMenu.position = { 580, 250, 0, 1 };
	mSMenu.SpriteDraw(baseDirectX);

	const float actionUIWid = 128.0f;
	const float actionUIHi = 32.0f;
	const float baseX = 550.0f;
	const float baseY = 300.0f;

	mSFront.position = { baseX, baseY, 0, 1 };
	mSFront.SpriteDraw(baseDirectX);
	ShowKeySprite(baseDirectX, 1, 0);

	mSBack.position = { baseX, baseY + 32.0f, 0, 1 };
	mSBack.SpriteDraw(baseDirectX);
	ShowKeySprite(baseDirectX, 0, 1);

	mSRight.position = { baseX, baseY + 64.0f, 0, 1 };
	mSRight.SpriteDraw(baseDirectX);
	ShowKeySprite(baseDirectX, 2, 2);

	mSLeft.position = { baseX, baseY + 96.0f, 0, 1 };
	mSLeft.SpriteDraw(baseDirectX);
	ShowKeySprite(baseDirectX, 3, 3);

	if (mInputReception)
	{
		mSInputWait.position = { static_cast<float>(window_width) / 2.0f - 64.0f, static_cast<float>(window_height) / 2.0f - 32.0f, 0, 1.0f };
		mSInputWait.SpriteDraw(baseDirectX);
	}
	if (mIsAlreadyInUse)
	{
		alreadyTimer++;
		if (alreadyTimer >= MaxAlreadyTime)
		{
			mIsAlreadyInUse = false;
			alreadyTimer = 0;
		}
		mSAlreadyUse.position = { static_cast<float>(window_width) / 2.0f - 64.0f, static_cast<float>(window_height) / 2.0f - 32.0f, 0, 1.0f };
		mSAlreadyUse.SpriteDraw(baseDirectX);
	}
}
