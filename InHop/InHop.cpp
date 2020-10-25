#include "Hooks.h"

DWORD WINAPI InHop(HMODULE hModule) {
	if (!Interfaces::init() || !Hooks::hook()) {
		return FALSE;
	}

	while (!GetAsyncKeyState(VK_HOME)) {
		Sleep(100);
	}

	Hooks::unHook();
	FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)InHop, hModule, 0, NULL);
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}