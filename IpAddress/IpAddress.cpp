#include <WinSock2.h> //windows>
#include <iphlpapi.h>
#include "IpAddress.h"
#include "../imgui/ImguiControl.h"

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

std::string IpAddress::ipAddress;

void IpAddress::GetIpAddress()
{
	DWORD i;
	PMIB_IPADDRTABLE pIpAddrTable = {};
	DWORD dwSize;
	DWORD dwRetVal;
	/*if (GetIpAddrTable(NULL, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER)
	{
		pIpAddrTable = (MIB_IPADDRTABLE*)malloc(dwSize);
	}

	if ((dwRetVal = GetIpAddrTable(pIpAddrTable, &dwSize, 0)) == NO_ERROR)
	{
		if (pIpAddrTable->dwNumEntries > 0)
		{
			ipAddress = inet_ntoa(*(struct in_addr*)&pIpAddrTable->table[0].dwAddr);
		}
	}*/

	Imgui::GetIp(ipAddress.c_str());
}
