#include <Windows.h>
#include <stdio.h>
#include "Hooks.h"

void MainThread() {
	OutputDebugString(L"MainThread");
	Hooks::setupHooks();
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MainThread, NULL, NULL, NULL);
	}

	return TRUE;
}
