#pragma once
#include <d3d12.h>

static D3D12_VIEWPORT viewport{};
static D3D12_RECT scissorrect{};
static int window_width = 1280;
static int window_height = 720;

class Viewport
{
public:
    
//画面サイズの変更とかもろもろ
	static void ViewPortUpdate()
	{
        viewport.Width = window_width;
        viewport.Height = window_height;
        viewport.TopLeftX = 0.0;
        viewport.TopLeftY = 0.0;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
	}
};

class Scissorrect
{
public:
    static void ScissorrectUpdate()
    {
        scissorrect.left = 0.0;
        scissorrect.right = scissorrect.left + window_width;
        scissorrect.top = 0.0;
        scissorrect.bottom = scissorrect.top + window_height;
    }
};