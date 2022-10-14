#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <WinSock2.h> //windows
#include <Windows.h>
#include "../UDP/udp.h"
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996) 

class VoiceUDPSender
{
	UDPClass udp;
public:
	void StartUp();
	void SendData();
	void EndSend();
};
