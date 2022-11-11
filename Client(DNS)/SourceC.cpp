#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include "ErrorHandling.h"
#include "Winsock2.h"  
#include "GetServerByHostName.h"
#pragma comment(lib, "WS2_32.lib")   // экспорт  WS2_32.dll

using namespace std;


void main()
{
	WSADATA wsaData;
	SOCKADDR_IN from;

	memset(&from, 0, sizeof(from));
	char name[] = "Hello", hostName[] = "LAPTOP-J9V5F5VS";
	int lfrom = sizeof(from);
	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("Startup: ", WSAGetLastError());

		if (GetServerByHostName(name, hostName, (sockaddr*)&from, &lfrom))
		{
			cout << "Connection successful" << endl << endl;
			cout << "Connected server: " << endl;
			cout << "Hostname: " << hostName << endl;
			cout << "IP: " << inet_ntoa(from.sin_addr) << endl;
			cout << "Port: " << htons(from.sin_port) << endl;
		}
		else
		{
			cout << "Wrong call name" << endl;
		}

		if (WSACleanup() == SOCKET_ERROR)
			throw SetErrorMsgText("Cleanup: ", WSAGetLastError());
	}
	catch (string errorMsgText)
	{
		cout << "WSAGetLastError: " << errorMsgText << endl;
	}
}

