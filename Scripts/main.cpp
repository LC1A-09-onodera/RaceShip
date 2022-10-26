#include "BaseDirectX/GameScene.h"
#include "Sound/Sound.h"
#include <dxgidebug.h>
#include "imgui/imgui_impl_dx12.h"
#include "imgui/imgui_impl_win32.h"

using namespace Microsoft::WRL;
using namespace DirectX;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "xaudio2.lib")

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	GameScene scene;
	scene.Init();

	while (true)  // ゲームループ
	{
		scene.SceneManageUpdateAndDraw();

		// DirectX毎フレーム処理　ここまで
		if (WindowsAPI::Qite)
		{
			break;
		}
	}
	Sound::xAudio2.Reset();
	// ウィンドウクラスを登録解除
	WindowsAPI::Clear();

	// Cleanup
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	return 0;
}
