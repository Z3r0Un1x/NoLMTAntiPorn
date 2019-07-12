#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>

using namespace std;

DWORD GetProcessID(LPCSTR ProcessName) {
	bool b;
	ULONG ProcessID = 0;
	HANDLE hProcessSnapshot = NULL;
	PROCESSENTRY32 ProcessEntry32 = { 0 };
	ProcessEntry32.dwSize = sizeof(PROCESSENTRY32);

	// Create the snapshot of all processes
	hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (!hProcessSnapshot) {
		return 0;
	}

	if (!Process32First(hProcessSnapshot, &ProcessEntry32)) {
		return 0;
	}
	do {
		try
		{
			b = strcmp(ProcessName, ProcessEntry32.szExeFile);
		}
		catch (const std::exception&)
		{
			throw;
		}
		if (b == 0) {
			ProcessID = ProcessEntry32.th32ProcessID;
			break;
		}
	} while (Process32Next(hProcessSnapshot, &ProcessEntry32));

	CloseHandle(hProcessSnapshot);

	return ProcessID;
}

VOID FetchingPassword(IN char lpDir[], OUT string * PasswordFetched)
{
	*PasswordFetched = "";

	const UINT count = MAX_PATH * 2;
	char szPassPath[count] = { NULL };

	strcpy_s(szPassPath, count, "");
	strcat_s(szPassPath, count, lpDir);
	strcat_s(szPassPath, count, "\\Microsoft.Win32.Core.dll");

	fstream f;
	f.open(szPassPath, ios::in);
	if (f.is_open())
	{
		f >> *PasswordFetched;
		f.close();
	}
	else
		cout << "ERR: Invalid file path!";
	if (szPassPath != NULL)
		SecureZeroMemory(szPassPath, sizeof(szPassPath));
	return VOID();
}

VOID FindLMTProcess(OUT bool* bRetVal)
{
	*bRetVal = EXIT_FAILURE;
	int flag = 0;

	if (GetProcessID("LMT Anti Porn.exe") != 0)
		flag++;

	if (flag > 0)
		*bRetVal = EXIT_SUCCESS;
	return VOID();
}


int __cdecl main(int argc, char *argv[])
{
	int retCode = -1;
	bool bVal = false;

	char InstallDir[MAX_PATH] = { NULL };
	string password = "";
	cout << "!NOLMTAntiPorn - Maintain by ZeroUnix!\n";
	cout << "Cwd: " << argv[0] << endl;
	cout << "Tracing for \"LMT Anti Porn\": ";
	FindLMTProcess(&bVal);
	if (bVal != EXIT_SUCCESS)
	{
		cout << "NOT FOUND.\n";
		retCode = EXIT_FAILURE;
		goto Cleanup;
	}
	cout << "FOUND.\n";
	cout << "Enter installation directory: ";
	scanf_s("%s", InstallDir, (unsigned)_countof(InstallDir));
	FetchingPassword(InstallDir, &password);
	if (password != "")
	{
		cout << "----------\nPassword: " << password << endl;
		cout << "----------\n";
		retCode = EXIT_SUCCESS;
	}
	else {
		retCode = EXIT_FAILURE;
		goto Cleanup;
	}

Cleanup:
	cout << "More at https://fb.me/ZeroUnix \n";
	system("pause");
	if (InstallDir != NULL)
		SecureZeroMemory(InstallDir, sizeof(InstallDir));	
	return retCode;
}

