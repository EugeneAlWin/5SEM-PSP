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

            SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, NULL);
            if (serverSocket  == INVALID_SOCKET) throw  SetErrorMsgText("socket:", WSAGetLastError());

            SOCKADDR_IN serv{};                    // параметры  сокета сервера
            serv.sin_family = AF_INET;           // используется IP-адресация  
            serv.sin_port = htons(2000);                   // TCP-порт 2000
            serv.sin_addr.s_addr = inet_addr("127.0.0.1");  // адрес сервера

       

            auto bindResult = bind(serverSocket, (LPSOCKADDR)&serv, sizeof(serv));
            if (bindResult == SOCKET_ERROR) throw  SetErrorMsgText("bind:", WSAGetLastError());

            auto listenResult = listen(serverSocket, SOMAXCONN);
            if (listenResult == SOCKET_ERROR) throw  SetErrorMsgText("listen:", WSAGetLastError());

            SOCKET clientSocket;    // сокет для обмена данными с клиентом 
            SOCKADDR_IN clnt;       // параметры  сокета клиента
            memset(&clnt, 0, sizeof(clnt)); // обнулить память
            int lclnt = sizeof(clnt);    // размер SOCKADDR_IN

            clientSocket = accept(serverSocket, (sockaddr*)&clnt, &lclnt);
            if (clientSocket == INVALID_SOCKET) throw  SetErrorMsgText("accept:", WSAGetLastError()); //	Разрешает попытку входящего подключения на сокете.

            char ibuf[50]{},                      //буфер ввода 
                obuf[50]{};                       //буфер вывода
            int  libuf = 0,                     //количество принятых байт
                lobuf = 0;                      //количество отправленных байь 

            while (true)
            {
                SOCKET libuf = recv(clientSocket, ibuf, sizeof(ibuf), NULL);
                if (libuf == SOCKET_ERROR) throw  SetErrorMsgText("recv:", WSAGetLastError());

                if (ibuf[0] == '\0') break;

                cout << ibuf << endl;

                int lobuf = send(clientSocket, ibuf, strlen(ibuf) + 1, NULL);
                if (lobuf == SOCKET_ERROR) throw  SetErrorMsgText("send:", WSAGetLastError());
            }

            if (closesocket(serverSocket) == SOCKET_ERROR) throw  SetErrorMsgText("closesocket:", WSAGetLastError());
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

