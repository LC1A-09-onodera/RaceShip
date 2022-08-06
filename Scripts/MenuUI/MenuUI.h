#pragma once
#include "../Sprite/Sprite.h"
#include "../BaseDirectX/BaseDirectX.h"
#include "../Rewired/Rewired.h"

class MenuUI
{
protected:
	//��������
	Sprite pouseBack;
	//�X�N���[�������l
	float pouseScroll = 0.0f;
	//�����܂ŃX�N���[���ł���悤��
	float maxpouseScroll = 0.0f;
	//��̗v�f�̑傫��
	float eachHeight = 32.0f;
	//������傫���Ȃ�X�N���[���ł���悤�ɂ���
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