#include "VoiceUDPRecive.h"
#include <iphlpapi.h>
#include <fstream>
#include "../imgui/ImguiControl.h"

UDPClass VoiceReciver::udp;
bool VoiceReciver::isShot;

bool VoiceReciver::isFront;
bool VoiceReciver::isBack;
bool VoiceReciver::isRight;
bool VoiceReciver::isLeft;
bool VoiceReciver::isPouse;
bool VoiceReciver::isWall;

void VoiceReciver::VoiceUDPUpdate()
{
	ReciveData();
}

void VoiceReciver::StartUp()
{
	udp.SetPortNum(8000);
	udp.StartUp();
}

void VoiceReciver::ReciveData()
{
	udp.Recive();
	std::string text = udp.GetResult();
	if (text == "") return;
	if (text != "")
	{
		int a = 0;
	}
	if (text == "SystemOpen")
	{
		Imgui::GetInstance()->SetWindowActive(true);
	}
	else if (text == "SystemClose")
	{
		Imgui::GetInstance()->SetWindowActive(false);
	}
	else if (text == "LinkStart")
	{

	}
	else if (text == "Explosion")
	{
		
	}
	else if (text == "Shot")
	{
		SetIsShot(true);
	}
	else if (text == "Left")
	{
		SetLeft(true);
	}
	else if (text == "Right")
	{
		SetRight(true);
	}
	else if (text == "Front")
	{
		SetFront(true);
	}
	else if (text == "Back")
	{
		SetBack(true);
	}
	else if (text == "Pouse")
	{
		SetPouse(true);
	}
	else if (text == "Shield")
	{
		SetWall(true);
	}

}

void VoiceReciver::EndRecive()
{
	udp.End();
}
