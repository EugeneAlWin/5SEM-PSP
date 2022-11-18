#include <iostream>
#include <Windows.h>
#include "ErrorHandling.h"

using namespace std;

int main()
{
	HANDLE hM; // дескриптор почтового ящика
	DWORD rb; // длина почитанного сообщения
	char rbuf[100]; // буфер ввода
	LPCTSTR SlotName = TEXT("\\\\.\\mailslot\\Box");
	try
	{
		if ((hM = CreateMailslot(
			SlotName, // имя слота
			300, // максимальный размер сообщения
			//MAILSLOT_WAIT_FOREVER, // ждать вечно
			180000, // 180000 milliseconds (180 sec = 3 min)
			NULL // атрибуты безопасности
		)) == INVALID_HANDLE_VALUE)
			throw SetPipeError("CreateMailslotError:", GetLastError());
		cout << "Mailslot server launched" << endl;
		cout << "Waiting for reading 3 minutes.." << endl;
		while (true)
		{
			if (!ReadFile(hM,
				rbuf, // буфер
				sizeof(rbuf), // размер буфера
				&rb, // прочитано
				NULL))
				throw SetPipeError("ReadFileError:", GetLastError());
			cout << rbuf << endl;
		}
		//..................................................................
		CloseHandle(hM);
	}
	catch (string ErrorPipeText)
	{
		cout << endl << ErrorPipeText;
	}
}