#include <stdio.h>
#include <string>
#include "VoiceUDPRecive.h"
#include <iphlpapi.h>
#include "../imgui/ImguiControl.h"

WSAData VoiceReciver::wsaData;
u_short VoiceReciver::port;
int VoiceReciver::sock;
struct sockaddr_in VoiceReciver::addr;

void VoiceReciver::VoiceUDPUpdate()
{
	ReciveData();
}

void VoiceReciver::StartUp()
{
	WSAStartup(MAKEWORD(2, 0), &wsaData);//MAKEWORD(2, 0)はバージョン
	sock = socket(AF_INET, SOCK_DGRAM, 0);//AF_INETはIPv4、SOCK_DGRAMはUDP通信、0は？

	addr.sin_family = AF_INET;  //IPv4
	addr.sin_port = htons(port);   //通信ポート番号設定
	addr.sin_addr.S_un.S_addr = INADDR_ANY; // INADDR_ANYはすべてのアドレスからのパケットを受信する
	bind(sock, (struct sockaddr*)&addr, sizeof(addr));
	u_long val = 1;
	ioctlsocket(sock, FIONBIO, &val);
}

void VoiceReciver::ReciveData()
{
	char buf[100];
	memset(buf, 0, 100);
	int data_size = recv(sock, buf, 100, 0);
	std::string tex = buf;
	if (data_size >= 0) {
		if (tex == "SystemOpen")
		{
			Imgui::SetWindowActive(true);
		}
		else if (tex == "SystemClose")
		{
			Imgui::SetWindowActive(false);
		}
		else if (tex == "LinkStart")
		{
			
		}
	}

}

void VoiceReciver::EndRecive()
{
	closesocket(sock);
	WSACleanup();
}
