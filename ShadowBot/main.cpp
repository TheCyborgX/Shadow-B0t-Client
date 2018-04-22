#include "main.hpp"

BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved
	)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Init, (PVOID)NULL, 0, NULL);
		//if (CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Init, (PVOID)NULL, 0, NULL) != NULL)
		//	MessageBox(NULL, L"Initialized!", L"Success!", MB_OK | MB_ICONASTERISK);

		return TRUE;
	}
	return FALSE;
}