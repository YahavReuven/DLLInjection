#pragma once

#include<Windows.h>

#define DLL_PATH "C:\\Projects\\C\\DLLInjector\\MyDll\\MyDll.dll"
#define KERNEL32_DLL "kernel32.dll"
#define LOAD_LIBRARY_A "LoadLibraryA"

/*Injects the process with the specified dll given in DLL_PATH.*/
int inject();

/*Writes the dll's path inside the process.*/
LPVOID WriteDllPathToProcess(HANDLE hProc);

/*Executes the dll inside the injected process.*/
void ExecuteInProcess(HANDLE hProc, LPVOID DllAddress);




