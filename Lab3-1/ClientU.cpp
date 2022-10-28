#include "SEMT.h"

int main()
{
    setlocale(LC_ALL, "rus");
    WSADATA wsaData;
    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) throw  SetErrorMsgText("Startup:", WSAGetLastError());

        SOCKET  cC = socket(AF_INET, SOCK_DGRAM, NULL);

        if (cC == INVALID_SOCKET) throw  SetErrorMsgText("socket:", WSAGetLastError());

        SOCKADDR_IN serv{};              // ���������  ������ �������
        serv.sin_family = AF_INET;    // ������������ ip-���������  
        serv.sin_port = htons(2000);   // ���� 2000
        serv.sin_addr.s_addr = inet_addr("127.0.0.1"); // ����� �������
        
        int  lobuf = 0;                         //���������� ������������  
        int lc = sizeof(serv);
        int libuf = 0,                     //���������� �������� ����
            count = 0;                      //���������� ���������

        cout << "Enter a number of itterations: ", cin >> count;

        char num[10],
            in[50]{};

        int t = clock();
        do
        {
            char out[50] = "Hello from ClientU ";
            _itoa(count, num, 10);
            strcat(out, num);

            lobuf = sendto(cC, out, strlen(out) + 1, NULL, (sockaddr*)&serv, sizeof(serv));
            if (lobuf == SOCKET_ERROR) throw  SetErrorMsgText("recv: ", WSAGetLastError());
			
            lobuf = recvfrom(cC, in, sizeof(in), NULL, (sockaddr*)&serv, &lc);
            if (lobuf  == SOCKET_ERROR) throw  SetErrorMsgText("recv:", WSAGetLastError());
            cout << in << endl;


        } while (--count);

        lobuf = sendto(cC, "", 1, NULL, (sockaddr*)&serv, sizeof(serv));
        if (lobuf == SOCKET_ERROR) throw  SetErrorMsgText("recv: ", WSAGetLastError());

        if (closesocket(cC) == SOCKET_ERROR) throw  SetErrorMsgText("closesocket: ", WSAGetLastError());
        if (WSACleanup() == SOCKET_ERROR) throw  SetErrorMsgText("Cleanup: ", WSAGetLastError());
    }
    catch (string errorMsgText)
    {
        cout << errorMsgText << endl;
    }
    return 0;

}