#include <iostream>
#include <Windows.h>
#include "ErrorHandling.h"
#include <tchar.h>

#define BUFSIZE 512

using namespace std;

int main()
{
	//LPCWSTR lpvMessage = TEXT("Darova, eto server");
	DWORD  NumOfBytesRead, NumOfBytesToWrite, NumOfBytesWritten;
	TCHAR  outBuffPtr[BUFSIZE]{};
	TCHAR checkBuf[BUFSIZE] = L"";
	BOOL   fSuccess = FALSE;

	HANDLE pipeHandle; // дескриптор канала

	bool multi = true; //////////////////////////////////////

	try
	{
		do
		{
			if ((
				pipeHandle = CreateNamedPipe(
				L"\\\\.\\pipe\\Tube",
				PIPE_ACCESS_DUPLEX, //дуплексный канал
					PIPE_TYPE_MESSAGE | PIPE_WAIT, // сообщения|синхронный
				1, // максимум 1 экземпляр канала
				NULL, // выходной буфер
				NULL,  // входной буфер
				INFINITE, // таймаут
				NULL // атрибуты безопасности
			)) == INVALID_HANDLE_VALUE)
				throw SetPipeError("create:", GetLastError());

			cout << "Waiting.." << endl;

			if (!ConnectNamedPipe(pipeHandle, NULL)) // ожидать клиента
				throw SetPipeError("connect:", GetLastError());

			cout << "Connected!" << endl;

			//..................................................................
			do
			{
				do
				{
					fSuccess = ReadFile(
						pipeHandle, // pipe handle
						outBuffPtr, // buffer to receive reply
						BUFSIZE * sizeof(TCHAR), // size of buffer
						&NumOfBytesRead, // number of bytes read
						NULL); // not overlapped
				} while (!fSuccess);

				if (!fSuccess)
				{
					throw SetPipeError("readfile:", GetLastError());
				}

				if (!wcscmp(outBuffPtr, checkBuf)) {
					break;
				}

				_tprintf(TEXT("%s\n"), outBuffPtr);

				//..................................................................

				NumOfBytesToWrite = (lstrlen(outBuffPtr) + 1) * sizeof(TCHAR);

				fSuccess = WriteFile(
					pipeHandle, // pipe handle
					outBuffPtr, // message
					NumOfBytesToWrite, // message length
					&NumOfBytesWritten, // bytes written
					NULL); // not overlapped

				if (!fSuccess)
				{
					throw SetPipeError("writefile:", GetLastError());
				}

			} while (wcscmp(outBuffPtr, checkBuf) && multi);

			cout << "Disconnected" << endl;

			DisconnectNamedPipe(pipeHandle);
			CloseHandle(pipeHandle);
		} while (true);
	}
	catch (string ErrorPipeText)
	{
		cout << endl << ErrorPipeText;

		//DisconnectNamedPipe(hPipe);
		//CloseHandle(hPipe);
	}

}