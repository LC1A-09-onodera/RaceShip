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

// Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	GameScene scene;
	scene.Init();

	while (true)  // �Q�[�����[�v
	{
		scene.SceneManageUpdateAndDraw();

		// DirectX���t���[�������@�����܂�
		if (WindowsAPI::Qite)
		{
			break;
		}
	}
	Sound::xAudio2.Reset();
	// �E�B���h�E�N���X��o�^����
	WindowsAPI::Clear();

	// Cleanup
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	return 0;
}
