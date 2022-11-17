// ClientNPt
#define _CRT_SECURE_NO_WARNING
#include <iostream>
#include <Windows.h>
#include "ErrorHandling.h"
#include <tchar.h>

#define BUFSIZE 512

using namespace std;

int main()
{
	DWORD  cbRead, cbToWrite, cbWritten;
	TCHAR  OutBuff[BUFSIZE] = TEXT("Hello from Client");
	TCHAR  zeroBuf[BUFSIZE] = TEXT("");
	LPTSTR InBuff = OutBuff;
	BOOL   fSuccess = FALSE;

	HANDLE pipeHandle; // дескриптор канала
	try
	{
		if ((pipeHandle = CreateFile(
			TEXT("\\\\LAPTOP-J9V5F5VS\\pipe\\Tube"),
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, NULL,
			NULL)) == INVALID_HANDLE_VALUE)
			throw SetPipeError("createfile:", GetLastError());

		cout << "Connected!" << endl;

		//..................................................................
		int num = 0;
		cout << "Enter number: ";
		cin >> num;

		clock_t start, stop;
		start = clock();

		for (int i = num; i > 0; i--)
		{
			char hello[] = "Hello from Client ";
			char numberstring[(((sizeof i) * CHAR_BIT) + 2) / 3 + 2];
			sprintf_s(numberstring, "%d", i);

			char result[BUFSIZE];   // array to hold the result.
			strcpy_s(result, hello); // copy string one into the result.
			strcat_s(result, numberstring);

			//size_t size = strlen(numberstring) + 1;
			wchar_t* wtext = new wchar_t[BUFSIZE];

			size_t outSize;
			mbstowcs_s(&outSize, wtext, BUFSIZE, result, BUFSIZE - 1);
			InBuff = wtext;

			cbToWrite = (lstrlen(InBuff) + 1) * sizeof(TCHAR);
			cout << "AllOk"<<endl;
			fSuccess = TransactNamedPipe(
				pipeHandle,                  // pipe handle 
				InBuff,              // message to server
				(lstrlen(InBuff) + 1) * sizeof(TCHAR), // message length 
				OutBuff,              // buffer to receive reply
				BUFSIZE * sizeof(TCHAR),  // size of read buffer
				&cbRead,                // bytes read
				NULL);                  // not overlapped 

			_tprintf(TEXT("%s\n"), OutBuff);

			if (!fSuccess)
			{
				throw SetPipeError("transact:", GetLastError());
				CloseHandle(pipeHandle);
			}
		}


		stop = clock();
		cout << (stop - start) << " ticks passed" << endl;

		InBuff = zeroBuf;

		cbToWrite = (lstrlen(InBuff) + 1) * sizeof(TCHAR);

		fSuccess = WriteFile(
			pipeHandle, // pipe handle
			InBuff, // message
			cbToWrite, // message length
			&cbWritten, // bytes written
			NULL); // not overlapped

		if (!fSuccess)
		{
			throw SetPipeError("writefile:", GetLastError());
		}

		cout << "Disconnected" << endl;
		CloseHandle(pipeHandle);
	}
	catch (string ErrorPipeText)
	{
		cout << endl << ErrorPipeText;
	}

}