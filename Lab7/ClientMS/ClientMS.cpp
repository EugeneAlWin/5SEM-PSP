#include <iostream>
#include <Windows.h>
#include "ErrorHandling.h"

using namespace std;

int main()
{
	HANDLE hM; // дескриптор почтового ящика
	DWORD wb; // длина записанного сообщения
	char wbuf[] = "Hello from Mailslot-client"; // буфер вывода
	LPCTSTR SlotName = TEXT("\\\\.\\mailslot\\Box");
	try
	{
		if ((hM = CreateFile(
			SlotName, // имя слота
			GENERIC_WRITE, // будем писать в ящик
			FILE_SHARE_READ, // разрешаем одновременно читать
			NULL, // атрибуты безопасности
			OPEN_EXISTING, // ящик уже есть
			NULL,	// атрибуты файла 
			NULL // шаблон файла 
		)) == INVALID_HANDLE_VALUE)
			throw SetPipeError("CreateFileError:", GetLastError());
		cout << "Mailslot client launched" << endl;

		clock_t start, stop;
		start = clock();

		for (int i = 0; i < 1000; i++)
			if (!WriteFile(
				hM, // дескриптор слота
				wbuf, // буфер
				sizeof(wbuf), // размер буфера
				&wb, // записано
				NULL // атрибуты асинхронной записи
			)) 
				throw SetPipeError("WriteFileError:", GetLastError());


		stop = clock();
		cout << stop - start << " ticks" << endl;
		//..................................................................
		CloseHandle(hM);
	}
	catch (string ErrorPipeText)
	{
		cout << endl << ErrorPipeText;
	}
}