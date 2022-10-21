#include "Input.h"
#include "../Tools/Rewired/Rewired.h"
#include "../imgui/ImguiControl.h"

std::shared_ptr<IDirectInput8> Input::dinput = nullptr;
IDirectInputDevice8 *Input::devkeyboard = nullptr;
DirectInput* Input::directInput = DirectInput::GetInstance();
int Input::mouseWheel = 0;
BYTE Input::keys[256] = {};
BYTE Input::oldkeys[256] = {};
void Input::Update()
{
	BaseDirectX::GetInstance()->result = devkeyboard->Acquire();
	for (int i = 0; i < 256; i++)
	{
		oldkeys[i] = keys[i];
		keys[i] = 0;
	}
	BaseDirectX::GetInstance()->result = devkeyboard->GetDeviceState(sizeof(keys), keys);

	directInput->UpdateInput();////////////////////////////////

	//�}�E�X�ʒu�̎擾
	//GetCursorPos(&mousePos);
	for (int i = 0; i < 10; i++)
	{
		oldMouseInput[i] = mouseInput[i];
		mouseInput[i] = false;
	}
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		mouseInput[VK_LBUTTON] = true;
	}
	if (GetAsyncKeyState(VK_RBUTTON))
	{
		mouseInput[VK_RBUTTON] = true;
	}
	if (GetAsyncKeyState(VK_CANCEL))
	{
		mouseInput[VK_CANCEL] = true;
	}
	OnMouseWheel();
}

void Input::Init()
{
	//directInput.reset(DirectInput::GetInstance());
}

void Input::KeySet(  WNDCLASSEX w, HWND hwnd)
{
	
	directInput->InputInit(w.hInstance,hwnd);//////////////////////////

	BaseDirectX::GetInstance()->result = DirectInput8Create(w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&dinput, nullptr);
	BaseDirectX::GetInstance()->result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	BaseDirectX::GetInstance()->result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);

	//��ʂ���O�ɂ���Ƃ��̂݃L�[�{�[�h���͂��󂯕t����
	//�f�o�C�X�����̃A�v�������Ő�L���Ȃ�
	//�E�B���X���L�[�̖�����
	BaseDirectX::GetInstance()->result = devkeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
}

bool Input::Key(BYTE key)
{
	return keys[key];
}

bool Input::Key(KeyCode key)
{
	return keys[(BYTE)key];
}

bool Input::KeyTrigger(BYTE key)
{
	return keys[key] && !oldkeys[key];
}

bool Input::KeyTrigger(KeyCode key)
{
	return keys[(BYTE)key] && !oldkeys[(BYTE)key];
}

bool Input::KeyEnd(BYTE key)
{
	return !keys[key] && oldkeys[key];
}

bool Input::KeyEnd(KeyCode key)
{
	return !keys[(BYTE)key] && oldkeys[(BYTE)key];
}

bool Input::MouseTrigger(MouseButton mouseinput)
{
	return mouseInput[(int)mouseinput] && !oldMouseInput[(int)mouseinput];
}

bool Input::Mouse(MouseButton mouseinput)
{
	return mouseInput[(int)mouseinput];
}

void Input::OnMouseWheel()
{
	mouseWheel = GET_WHEEL_DELTA_WPARAM(WindowsAPI::msg.wParam) / WHEEL_DELTA;
	Imgui::GetInstance()->mouseWheel = mouseWheel;
	WindowsAPI::msg.wParam = 1;
}
