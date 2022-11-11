#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Winsock2.h"  
#include "ErrorHandling.h"

bool GetServerByHostName(char* name, char* hostName, sockaddr* from, int* flen)
{
	SOCKET sock;

	hostent* servInfo;
	memset(&servInfo, 0, sizeof(servInfo));
	servInfo = gethostbyname(hostName);

	in_addr* addr_list = (in_addr*)*(servInfo->h_addr_list);

	((SOCKADDR_IN*)from)->sin_family = AF_INET;
	((SOCKADDR_IN*)from)->sin_port = htons(2000);
	((SOCKADDR_IN*)from)->sin_addr = *addr_list;

	int lbuf, optval = 1;
	char ibuf[50];

	if ((sock = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
		throw SetErrorMsgText("socket: ", WSAGetLastError());

	if (lbuf = sendto(sock, name, strlen(name) + 1, NULL, from, *flen) == SOCKET_ERROR)
		throw SetErrorMsgText("sendto: ", WSAGetLastError());

	if (lbuf = recvfrom(sock, ibuf, sizeof(ibuf), NULL, from, flen) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAETIMEDOUT)
			return false;
		else
			throw SetErrorMsgText("recvfrom: ", WSAGetLastError());
	}

	if (closesocket(sock) == SOCKET_ERROR)
		throw SetErrorMsgText("closesocket: ", WSAGetLastError());

	return strcmp(ibuf, name) == 0;
}