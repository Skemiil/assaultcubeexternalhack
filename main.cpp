#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

DWORD GetPID(const char* ProcessName) {
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);


	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	Process32First(processesSnapshot, &processInfo);
	if (!strcmp(processInfo.szExeFile, ProcessName))
	{
		CloseHandle(processesSnapshot);
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (!strcmp(processInfo.szExeFile, ProcessName))
		{
			CloseHandle(processesSnapshot);
		}
	}
	CloseHandle(processesSnapshot);
	return processInfo.th32ProcessID;
}

MODULEENTRY32 GetModule(const char* moduleName, unsigned long ProcessID)
{
	MODULEENTRY32 modEntry = { 0 };

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcessID);

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 curr = { 0 };

		curr.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnapshot, &curr))
		{
			do
			{
				if (!strcmp(curr.szModule, moduleName))
				{
					modEntry = curr;
					break;
				}
			} while (Module32Next(hSnapshot, &curr));
		}
		CloseHandle(hSnapshot);
	}
	return modEntry;
}

int main()
{
	for (int i = 1; i > 0; i++) {

		std::cout << "get fucked!\n";
		unsigned long long pid = GetPID("ac_client.exe");
		MODULEENTRY32 module = GetModule("ac_client.exe", pid);
		HANDLE phandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		std::cout << "PID: " << pid << std::endl;

		unsigned long long result;
		unsigned long long ammoresult;
		unsigned long long grenadesresult;
		int desiredhealth = 999;
		int desiredammo = 999;
		int desiredgrenades = 999;
		ReadProcessMemory(phandle, (void*)((unsigned long long)module.modBaseAddr + 0x0010F4F4), &result, sizeof(result), 0);
		WriteProcessMemory(phandle, (void*)((unsigned long long)result + 0xF8), &desiredhealth, sizeof(desiredhealth), 0);
		ReadProcessMemory(phandle, (void*)((unsigned long long)result + 0xF8), &result, sizeof(result), 0);

		ReadProcessMemory(phandle, (void*)((unsigned long long)module.modBaseAddr + 0x0010F4F4), &ammoresult, sizeof(ammoresult), 0);
		WriteProcessMemory(phandle, (void*)((unsigned long long)ammoresult + 0x150), &desiredammo, sizeof(desiredammo), 0);
		ReadProcessMemory(phandle, (void*)((unsigned long long)ammoresult + 0x150), &ammoresult, sizeof(ammoresult), 0);

		ReadProcessMemory(phandle, (void*)((unsigned long long)module.modBaseAddr + 0x0010F4F4), &grenadesresult, sizeof(grenadesresult), 0);
		WriteProcessMemory(phandle, (void*)((unsigned long long)grenadesresult + 0x158), &desiredgrenades, sizeof(desiredgrenades), 0);
		ReadProcessMemory(phandle, (void*)((unsigned long long)grenadesresult + 0x158), &grenadesresult, sizeof(grenadesresult), 0);
	}
}
