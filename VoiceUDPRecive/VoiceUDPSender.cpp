#include "VoiceUDPSender.h"
#include <string>

void VoiceUDPSender::StartUp()
{
	WSACleanup();
	WSAStartup(MAKEWORD(2, 0), &wsaData);   //MAKEWORD(2, 0)はwinsockのバージョン2.0ってこと
	sock = static_cast<int>(socket(AF_INET, SOCK_DGRAM, 0));  //AF_INETはIPv4、SOCK_DGRAMはUDP通信、0は？
	addr.sin_family = AF_INET;
	addr.sin_port = htons(50008);// 待ち受けポート番号を50008にする
	std::string add = "127.0.0.1";
	addr.sin_addr.S_un.S_addr = inet_addr(add.c_str());// 送信アドレスを設定
}

void VoiceUDPSender::SendData()
{
	for (int i = 0; i < 100; i++)
	{
		char data[] = "test";
		//データ送信
		sendto(sock, data, sizeof(data), 0, (struct sockaddr*)&addr, sizeof(addr));//addrに文字列送信
		printf("send data!\n");
	}
}

void VoiceUDPSender::EndSend()
{
	closesocket(sock);
	WSACleanup();
}
