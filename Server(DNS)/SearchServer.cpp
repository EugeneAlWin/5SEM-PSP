#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include "ErrorHandling.h"
#include "Winsock2.h" 

using namespace std;
void SearchServer(char* name)
{
	SOCKADDR_IN from;
	memset(&from, 0, sizeof(from));
	int lfrom = sizeof(from);
	SOCKET sock;
	int lbuf;
	int optval = 3000;
	if ((sock = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
	{
		throw SetErrorMsgText("socket: ", WSAGetLastError());
	}
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(optval)) == SOCKET_ERROR)
		throw SetErrorMsgText("opt: ", WSAGetLastError());
	SOCKADDR_IN all{};
	all.sin_family = AF_INET;
	all.sin_port = htons(2000);
	all.sin_addr.s_addr = INADDR_BROADCAST;
	char ibuf[50];
	if (lbuf = sendto(sock, name, strlen(name) + 1, NULL, (sockaddr*)&all, sizeof(all)) == SOCKET_ERROR)
	{
		throw SetErrorMsgText("sendto: ", WSAGetLastError());
	}
	if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&optval, sizeof(optval)) == SOCKET_ERROR)
		throw SetErrorMsgText("opt: ", WSAGetLastError());
	if (lbuf = recvfrom(sock, ibuf, sizeof(ibuf), NULL, (sockaddr*)&from, &lfrom) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAETIMEDOUT)
			throw SetErrorMsgText("recvfrom: ", WSAGetLastError());
	}
	if (lbuf == 0)
	{
		cout << "Server socket: " << endl;
		cout << "IP: " << inet_ntoa(from.sin_addr) << endl;
		cout << "Port: " << htons(from.sin_port) << endl;
	}
	if (closesocket(sock) == SOCKET_ERROR)
	{
		throw SetErrorMsgText("closesocket: ", WSAGetLastError());
	}
}