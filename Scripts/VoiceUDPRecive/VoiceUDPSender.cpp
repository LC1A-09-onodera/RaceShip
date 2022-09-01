#include "VoiceUDPSender.h"
#include <string>

void VoiceUDPSender::StartUp()
{
	WSACleanup();
	udp.StartUp();
}

void VoiceUDPSender::SendData()
{
	char data[] = "test";
	//データ送信
	udp.SendData(data);
	printf("send data!\n");
}

void VoiceUDPSender::EndSend()
{
	udp.End();
}
