#include "SEMT.h"

int main()
{
    setlocale(LC_ALL, "rus");
    WSADATA wsaData;
    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) throw  SetErrorMsgText("Startup:", WSAGetLastError());

        SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, NULL);

        if (clientSocket == INVALID_SOCKET) throw  SetErrorMsgText("socket:", WSAGetLastError());

        SOCKADDR_IN serv{};                    // ���������  ������ �������
        serv.sin_family = AF_INET;           // ������������ IP-���������  
        serv.sin_port = htons(2000);                   // TCP-���� 2000
        serv.sin_addr.s_addr = inet_addr("127.0.0.1");  // ����� �������

        auto connectResult = connect(clientSocket, (sockaddr*)&serv, sizeof(serv));
        if (connectResult == SOCKET_ERROR) throw  SetErrorMsgText("connect:", WSAGetLastError()); //������������� ����������� � ���������� ������.

        char ibuf[50]{},                      //����� ����� 
            obuf[50]{};                       //����� ������
        int  libuf = 0,                     //���������� �������� ����
            lobuf = 0,                      //���������� ������������ ����
            count = 0;                      //���������� ���������

        cout << "������� ����� ��������: ", cin >> count;

        char num[10],
            in[50];
        int t = clock();

        do
        {
            char out[50] = "������ �� ������� ";
            _itoa(count, num, 10);
            strcat(out, num);
			
            lobuf = send(clientSocket, out, strlen(out) + 1, NULL);
            if (lobuf == SOCKET_ERROR) throw  SetErrorMsgText("send:", WSAGetLastError());

            libuf = recv(clientSocket, in, sizeof(in), NULL);
            if (libuf == SOCKET_ERROR) throw  SetErrorMsgText("recv:", WSAGetLastError());
            cout << in << endl;


        } while (--count);

        cout << "Ticks: " << clock() - t << endl;
        
        lobuf = send(clientSocket, "", 1, NULL);
        if (lobuf == SOCKET_ERROR) throw  SetErrorMsgText("send:", WSAGetLastError());
        if (closesocket(clientSocket) == SOCKET_ERROR) throw  SetErrorMsgText("closesocket:", WSAGetLastError());
        if (WSACleanup() == SOCKET_ERROR) throw  SetErrorMsgText("Cleanup:", WSAGetLastError());
    }
    catch (string errorMsgText)
    {
        cout << errorMsgText << endl;
    }
    return 0;

}
