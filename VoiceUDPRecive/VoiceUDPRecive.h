#pragma once
#include <WinSock2.h> //windows
#include <string>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

class VoiceReciver
{
public:
	static WSAData wsaData;
	static u_short port;
	static int sock;

	//ƒAƒhƒŒƒX‚ÌŠi”[
	static struct sockaddr_in addr;
	static void VoiceUDPUpdate();
	static void StartUp();
	static void ReciveData();
	static void EndRecive();
};
