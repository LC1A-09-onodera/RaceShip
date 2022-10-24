#include "WinAPI.h"
#include "../imgui/imgui_impl_dx12.h"
#include "../imgui/imgui_impl_win32.h"

HWND WindowsAPI::hwnd{};
MSG WindowsAPI::msg{};
WNDCLASSEX WindowsAPI::w{};
bool WindowsAPI::Qite;
int WindowsAPI::intarval = 0;
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);
float WindowsAPI::rate;

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    // ���b�Z�[�W�ŕ���
    switch (msg) {
    case WM_DESTROY: // �E�B���h�E���j�����ꂽ
        PostQuitMessage(0); // OS�ɑ΂��āA�A�v���̏I����`����
        return 0;
    }
    ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
    return DefWindowProc(hwnd, msg, wparam, lparam);// �W���̏������s��
}

void WindowsAPI::Set()
{
    //WNDCLASSEX w{}; // �E�B���h�E�N���X�̐ݒ�
    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = (WNDPROC)WindowProc; // �E�B���h�E�v���V�[�W����ݒ�
    w.lpszClassName = L"Race Ship"; // �E�B���h�E�N���X��
    w.hInstance = GetModuleHandle(nullptr); // �E�B���h�E�n���h��
    w.hCursor = LoadCursor(NULL, IDC_ARROW); // �J�[�\���w��
    // �E�B���h�E�N���X��OS�ɓo�^
    RegisterClassEx(&w);
    // �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
    RECT wrc = { 0, 0, window_width, window_height };
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // �����ŃT�C�Y�␳

    hwnd = CreateWindow(
        w.lpszClassName, // �N���X��
        L"Race Ship",         // �^�C�g���o�[�̕���
        WS_OVERLAPPEDWINDOW,        // �W���I�ȃE�B���h�E�X�^�C��
        CW_USEDEFAULT,              // �\��X���W�iOS�ɔC����j
        CW_USEDEFAULT,              // �\��Y���W�iOS�ɔC����j
        wrc.right - wrc.left,       // �E�B���h�E����
        wrc.bottom - wrc.top,   // �E�B���h�E�c��
        nullptr,                // �e�E�B���h�E�n���h��
        nullptr,                // ���j���[�n���h��
        w.hInstance,            // �Ăяo���A�v���P�[�V�����n���h��
        nullptr);               // �I�v�V����

    // �E�B���h�E�\��
    ShowWindow(hwnd, SW_SHOW);
}

void WindowsAPI::Clear()
{
    UnregisterClass(w.lpszClassName, w.hInstance);
}

void WindowsAPI::CheckMsg()
{
    // ���b�Z�[�W������H
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg); // �L�[���̓��b�Z�[�W�̏���
        DispatchMessage(&msg); // �v���V�[�W���Ƀ��b�Z�[�W�𑗂�
    }
    Update();
}

void WindowsAPI::Update()
{
    auto hdc = GetDC(hwnd);
    rate = static_cast<float>(GetDeviceCaps(hdc, VREFRESH));
    if (rate < 1)
    {
        int a = 0;
    }
    if (intarval == 0)
    {
        intarval = static_cast<int>(rate / 60.0f);
    }
    if (msg.message == WM_QUIT)
    {
        Qite = true;
    }
}

void WindowsAPI::Gethwnd()
{

}

POINT WindowsAPI::GetMousePos()
{
    POINT result;
    GetCursorPos(&result);
    ScreenToClient(hwnd, &result);

    return result;
}


