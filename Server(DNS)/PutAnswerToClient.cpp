#include "ErrorHandling.h"
#include "Winsock2.h"  
bool PutAnswerToClient(char* name, short port, struct sockaddr* to, int* lto)
{
	int lbuf;
	if (lbuf = sendto(port, name, strlen(name) + 1, NULL, to, *lto) == SOCKET_ERROR)
		throw SetErrorMsgText("sendto: ", WSAGetLastError());

	return true;
}