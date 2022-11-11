#pragma once
#include "Winsock2.h"  

bool GetServerByHostName(char* name, char* hostName, sockaddr* from, int* flen);
