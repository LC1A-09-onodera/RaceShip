#pragma once
#include "../Sprite/Sprite.h"

class RenderTarget
{
public:
	//�}���`�����_�����O�s�����߂ɔz��
	static const int renderNum = 1;
	ComPtr<ID3D12Resource> texBuff[renderNum];
	static const float clearColor[4];
	//RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	void TexInit();
	void RTVInit();
};
