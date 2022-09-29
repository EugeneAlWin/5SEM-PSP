#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <ctime>
#include "ErrorHandling.h"
#include "Winsock2.h"  
#pragma comment(lib, "WS2_32.lib")   // экспорт  WS2_32.dll

using namespace std;

bool GetRequestFromClient(char*, short, struct sockaddr*, int*); 
bool PutAnswerToClient(char*, short, struct sockaddr*, int*);

int main()
{
	WSADATA wsaData;
	SOCKET sS;
	SOCKET allS;
	SOCKADDR_IN serv;
	SOCKADDR_IN from;
	
	char name[50] = "Hello";
	serv.sin_family = AF_INET;           // используется IP-адресация  
	serv.sin_port = htons(2000);          // порт 2000
	serv.sin_addr.s_addr = INADDR_ANY;   // любой собственный IP-адрес 
	int servs = 0;
	memset(&from, 0, sizeof(from));   // обнулить память
	int lfrom = sizeof(from);

	
	try 
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw  SetErrorMsgText("Startup:", WSAGetLastError());

		// Searching servers
		if ((allS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw  SetErrorMsgText("Socket:", WSAGetLastError());

		cout << "Search for servers..." << endl;

		if (closesocket(allS) == SOCKET_ERROR)
			throw  SetErrorMsgText("CloseSocket:", WSAGetLastError());
		
		if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw  SetErrorMsgText("Socket:", WSAGetLastError());
		if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
			throw  SetErrorMsgText("Bind:", WSAGetLastError());
		do 
		{
			if (GetRequestFromClient(name, sS, (sockaddr*)&from, &lfrom))
			{
				cout << endl;
				cout << "Client socket:" << endl;
				cout << "IP: " << inet_ntoa(from.sin_addr) << endl;
				cout << "Port: " << htons(from.sin_port) << endl;
				cout << endl;
				if (PutAnswerToClient(name, sS, (sockaddr*)&from, &lfrom))
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

bool GetRequestFromClient(char* name, short port, struct sockaddr* from, int* flen)
{
	int lbuf;
	char ibuf[50] = "";
	cout << "Wait message..." << endl;
	while (true)
	{
		if (lbuf = recvfrom(port, ibuf, sizeof(ibuf), NULL, from, flen) == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAETIMEDOUT)
				return false;
			else
				throw SetErrorMsgText("recvfrom: ", WSAGetLastError());
		}
		if (strcmp(ibuf, name) == 0)
			return true;
		else
			return false;
	}
}

bool PutAnswerToClient(char* name, short port, struct sockaddr* to, int* lto) 
{
	int lbuf;
	if (lbuf = sendto(port, name, strlen(name) + 1, NULL, to, *lto) == SOCKET_ERROR)
		throw SetErrorMsgText("sendto: ", WSAGetLastError());

	return true;
}