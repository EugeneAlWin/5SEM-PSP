#include "SEMT.h"

int main()
{
    setlocale(LC_ALL, "rus");
    WSADATA wsaData;
    try
    {
        while (true)
        {
            if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) throw  SetErrorMsgText("Startup:", WSAGetLastError());

            SOCKET  sS = socket(AF_INET, SOCK_DGRAM, NULL);

            if (sS == INVALID_SOCKET) throw  SetErrorMsgText("socket:", WSAGetLastError());

            SOCKADDR_IN serv{};                    // ���������  ������ sS
            serv.sin_family = AF_INET;           // ������������ IPv4-���������  
            serv.sin_port = htons(2000);         // ���� 2000
            serv.sin_addr.s_addr = INADDR_ANY;   // ����� ����������� IP-����� 
            //serv.sin_addr.s_addr = inet_addr("192.168.1.130");

            auto bindRes = bind(sS, (LPSOCKADDR)&serv, sizeof(serv));
            if (bindRes == SOCKET_ERROR) throw  SetErrorMsgText("bind:", WSAGetLastError());

            SOCKADDR_IN clnt;               // ���������  ������ �������
            memset(&clnt, 0, sizeof(clnt)); // �������� ������

            int lc = sizeof(clnt);
            char ibuf[50];                  //����� ����� 
            int  lb = 0;                    //���������� �������� ����

            while (true)
            {
                lb = recvfrom(sS, ibuf, sizeof(ibuf), NULL, (sockaddr*)&clnt, &lc);
                if (lb == SOCKET_ERROR) throw  SetErrorMsgText("recv:", WSAGetLastError());
                //Sleep(1000);
                if (ibuf[0] == '\0') break;
                cout << ibuf << endl;
                lb = sendto(sS, ibuf, strlen(ibuf) + 1, NULL, (sockaddr*)&clnt, sizeof(clnt));
                if (lb == SOCKET_ERROR) throw  SetErrorMsgText("recv: ", WSAGetLastError());
            }



            if (closesocket(sS) == SOCKET_ERROR) throw  SetErrorMsgText("closesocket:", WSAGetLastError());

            if (WSACleanup() == SOCKET_ERROR) throw  SetErrorMsgText("Cleanup:", WSAGetLastError());

            cout << "Lost connection" << endl << "Wait for another connection" << endl;
        }
    }
    catch (string errorMsgText)
    {
        cout << errorMsgText << endl;
    }
    return 0;

}