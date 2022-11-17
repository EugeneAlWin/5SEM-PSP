#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include "ErrorHandling.h"
#include "Winsock2.h"  
#include "GetRequestFromClient.h"
#include "PutAnswerToClient.h"
#include "SearchServer.h"
#pragma comment(lib, "WS2_32.lib")   // экспорт  WS2_32.dll

using namespace std;

int main()
{
	WSADATA wsaData;
	SOCKET sS;
	SOCKADDR_IN serv{}, from{};

	char name[50] = "Hello";
	serv.sin_family = AF_INET;           // используется IP-адресация  
	serv.sin_port = htons(2000);          // порт 2000
	serv.sin_addr.s_addr = INADDR_ANY;   // любой собственный IP-адрес 

	memset(&from, 0, sizeof(from));   // обнулить память
	int lfrom = sizeof(from);

	char hostname[25];
	hostent* clntInfo; // информация о клиенте 
	memset(&clntInfo, 0, sizeof(clntInfo));

	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw  SetErrorMsgText("Startup:", WSAGetLastError());

		cout << "Search servers..." << endl;
		SearchServer(name);

		if (WSACleanup() == SOCKET_ERROR)
			throw  SetErrorMsgText("Cleanup:", WSAGetLastError());

		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw  SetErrorMsgText("Startup:", WSAGetLastError());
		if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw  SetErrorMsgText("Socket:", WSAGetLastError());
		if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
			throw  SetErrorMsgText("Bind:", WSAGetLastError());
		if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
			throw SetErrorMsgText("gethostname: ", WSAGetLastError());
		cout << "Server hostname: " << hostname << endl;
		do
		{
			if (GetRequestFromClient(name, (short)sS, (sockaddr*)&from, &lfrom))
			{
				clntInfo = gethostbyaddr((const char*)&from.sin_addr, 4, AF_INET);
				cout << endl;
				cout << "Client socket:" << endl;
				cout << "Hostname: " << clntInfo->h_name << endl;
				cout << "IP: " << inet_ntoa(from.sin_addr) << endl;
				cout << "Port: " << htons(from.sin_port) << endl;
				cout << endl;
				if (PutAnswerToClient(name, (short)sS, (sockaddr*)&from, &lfrom))
				{
					cout << "Success" << endl;
				}
			}
			else
				cout << "Wrong call name" << endl;
		} while (true);

		if (closesocket(sS) == SOCKET_ERROR)
			throw  SetErrorMsgText("CloseSocket:", WSAGetLastError());
		if (WSACleanup() == SOCKET_ERROR)
			throw  SetErrorMsgText("Cleanup:", WSAGetLastError());
	}
	catch (string errorMsgText)
	{
		cout << "\n" << errorMsgText << endl;
	}
}