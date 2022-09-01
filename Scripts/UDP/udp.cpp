#include "udp.h"

void UDPClass::SetPortNum(int f_port)
{
	this->port = static_cast<u_short>(f_port);
}

void UDPClass::StartUp()
{
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)//MAKEWORD(2, 0)�̓o�[�W����
	{
		sock = static_cast<int>(socket(AF_INET, SOCK_DGRAM, 0));//AF_INET��IPv4�ASOCK_DGRAM��UDP�ʐM�A0�́H
	}
	addr.sin_family = AF_INET;  //IPv4
	addr.sin_port = htons(port);   //�ʐM�|�[�g�ԍ��ݒ�
	addr.sin_addr.S_un.S_addr = INADDR_ANY; // INADDR_ANY�͂��ׂẴA�h���X����̃p�P�b�g����M����
	bind(sock, (struct sockaddr*)&addr, sizeof(addr));
	u_long val = 1;
	ioctlsocket(sock, FIONBIO, &val);
}

void UDPClass::SendData(const char* data)
{
	sendto(sock, data, sizeof(data), 0, (struct sockaddr*)&addr, sizeof(addr));//addr�ɕ����񑗐M
}

void UDPClass::Recive()
{
	char buf[100] = {};
	memset(buf, 0, 100);
	//int data_size = recv(sock, buf, 100, 0);
	tex = buf;
}

void UDPClass::End()
{
	closesocket(sock);
	WSACleanup();
}