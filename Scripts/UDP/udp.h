#pragma once
#include <WinSock2.h> //windows
#include <string>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

class UDPClass
{
public:
	WSAData wsaData = {};
	u_short port = u_short();
	int sock = -1;
	//�A�h���X�̊i�[
	struct sockaddr_in addr = sockaddr_in();
	//�󂯎�������ʂ̊i�[
	std::string tex;

	void SetPortNum(int f_port);
	void StartUp();
	void SendData(const char *data);
	void Recive();
	void End();
	std::string GetResult() {return tex;}
};