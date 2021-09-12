#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>
#include "dll_injector.h"


LPVOID WriteDllPathToProcess(HANDLE hProc) {

	LPVOID DllAddressProc = NULL;
	BOOL iResult = FALSE;

	DllAddressProc = VirtualAllocEx(hProc, NULL, sizeof(DLL_PATH), MEM_COMMIT | MEM_RESERVE,
		PAGE_READWRITE);

	if (!DllAddressProc) {
		printf("couldn't allocate the dll path to the process, with error %lu.\n", GetLastError());
		return NULL;
	}

	iResult = WriteProcessMemory(hProc, DllAddressProc, DLL_PATH, sizeof(DLL_PATH), NULL);

	if (!iResult) {
		printf("couldn't write to the process memory, with error %lu.\n", GetLastError());
		return NULL;
	}

	return DllAddressProc;
}


void ExecuteInProcess(HANDLE hProc, LPVOID DllAddress) {

	HMODULE Kernel32hModule = NULL;
	LPVOID LlaAddress = NULL;
	HANDLE ThreadHandle = NULL;

	Kernel32hModule = LoadLibraryA(KERNEL32_DLL);

	if (!Kernel32hModule) {
		printf("couldn't load the file, with error %lu.\n", GetLastError());
		return;
	}

	LlaAddress = GetProcAddress(Kernel32hModule, LOAD_LIBRARY_A);

	if (!LlaAddress) {
		printf("couldn't get the LoadLibraryA address, with error %lu.\n", GetLastError());
		return;
	}

	ThreadHandle = CreateRemoteThread(hProc, NULL, 0, LlaAddress, DllAddress, 0, NULL);

	if (!ThreadHandle) {
		printf("couldn't create remote thread, with error %lu.\n", GetLastError());
		return;
	}

	FreeLibrary(Kernel32hModule);

}


int inject() {

	DWORD pid = 0;
	HANDLE hProc = NULL;
	LPVOID dllAddress = NULL;

	puts("please enter process id: ");
	scanf("%lu", &pid);

	hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	if (!hProc) {
		printf("couldn't open process %lu, with error %lu.\n", pid, GetLastError());
	}

	dllAddress = WriteDllPathToProcess(hProc);

	if (!dllAddress) {
		return;
	}

	ExecuteInProcess(hProc, dllAddress);

	CloseHandle(hProc);

}