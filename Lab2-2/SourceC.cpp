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

        SOCKADDR_IN serv{};                    // параметры  сокета сервера
        serv.sin_family = AF_INET;           // используется IP-адресация  
        serv.sin_port = htons(2000);                   // TCP-порт 2000
        serv.sin_addr.s_addr = inet_addr("127.0.0.1");  // адрес сервера

        auto connectResult = connect(clientSocket, (sockaddr*)&serv, sizeof(serv));
        if (connectResult == SOCKET_ERROR) throw  SetErrorMsgText("connect:", WSAGetLastError()); //Устанавливает подключение к указанному сокету.

        char ibuf[50]{},                      //буфер ввода 
            obuf[50]{};                       //буфер вывода
        int  libuf = 0,                     //количество принятых байт
            lobuf = 0,                      //количество отправленных байт
            count = 0;                      //количество иттераций

        cout << "Введите число итераций: ", cin >> count;

        char num[10],
            in[50];
        int t = clock();

        do
        {
            char out[50] = "Привет от клиента ";
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
