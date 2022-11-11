#include <iostream>
#include "ErrorHandling.h"
#include "Winsock2.h"  

using namespace std;
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
		return strcmp(ibuf, name) == 0;
	}
}