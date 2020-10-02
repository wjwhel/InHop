// dllmain.cpp : Defines the entry point for the DLL application.
#define _CRT_SECURE_NO_WARNINGS
#include "Windows.h"
#include "sdk.h"
#include <iostream>

DWORD WINAPI Thread(HMODULE hModule);
bool findInterfaces();
bool setHooks();
void unSetHooks();
void* CaptureInterface(const wchar_t* module, const char* name);
void __stdcall createMove(float inputSampleTime, CUserCmd* cmd);

namespace Interfaces
{
	IBaseClientDLL* client = nullptr;
	IClientEntityList* entityList = nullptr;
	IVEngineClient* engine = nullptr;
	ClientMode* clientMode = nullptr;
}

namespace Hooks
{
	typedef void(__thiscall* createMove_t)(ClientMode*, float inputSampleTime, CUserCmd* cmd);
	uintptr_t* pNewPanelTable, * pOldPanelTable;
}

using namespace Interfaces;
using namespace Hooks;
using namespace std;

bool findInterfaces()
{
	//clientBase = (DWORD)GetModuleHandleW(L"client_panorama.dll");
	client = reinterpret_cast<IBaseClientDLL*>(CaptureInterface(L"client.dll", "VClient018"));
	entityList = reinterpret_cast<IClientEntityList*>(CaptureInterface(L"client.dll", "VClientEntityList003"));
	engine = reinterpret_cast<IVEngineClient*>(CaptureInterface(L"engine.dll", "VEngineClient014"));
	clientMode = **reinterpret_cast<ClientMode***>((*reinterpret_cast<uintptr_t**>(client))[10] + 5);

	return nullptr != client
		&& nullptr != entityList
		&& nullptr != engine
		&& nullptr != clientMode;
}

bool setHooks()
{
	const auto fnEstimateTableLength = [](uintptr_t* pTable)
	{
		auto sReturn = std::size_t(0u);
		MEMORY_BASIC_INFORMATION mbiTable{ };

		while (NULL != VirtualQuery(reinterpret_cast<LPCVOID>(pTable[sReturn]), &mbiTable, sizeof mbiTable) &&
			mbiTable.BaseAddress != nullptr &&
			mbiTable.Type != NULL &&
			mbiTable.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY) &&
			!(mbiTable.Protect & (PAGE_GUARD | PAGE_NOACCESS)))
			sReturn++;
		return sReturn;
	};

	pOldPanelTable = *reinterpret_cast<uintptr_t**>(clientMode);

	if (pOldPanelTable == nullptr) {
		return false;
	}

	const auto length = fnEstimateTableLength(pOldPanelTable);

	if (length == NULL) {
		return false;
	}

	pNewPanelTable = new uintptr_t[length];

	memcpy(pNewPanelTable, pOldPanelTable, length * sizeof(uintptr_t));

	*reinterpret_cast<uintptr_t**>(clientMode) = pNewPanelTable;

	pNewPanelTable[24] = uintptr_t(createMove);
}

void unSetHooks()
{
	*reinterpret_cast<uintptr_t**>(clientMode) = pOldPanelTable;

	delete pNewPanelTable;
}

DWORD WINAPI Thread(HMODULE hModule) {
	//AllocConsole();
	//freopen("CONOUT$", "w", stdout);

	if (!findInterfaces() || !setHooks()) {
		return FALSE;
	}

	while (!GetAsyncKeyState(VK_HOME)) {
		Sleep(100);
	}

	unSetHooks();

	//FreeConsole();
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
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread, hModule, 0, 0);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


static void __stdcall createMove(float inputSampleTime, CUserCmd* cmd)
{
	static auto fnOriginal = createMove_t(pOldPanelTable[24]);

	fnOriginal(clientMode, inputSampleTime, cmd);

	if (!cmd->commandNumber)
		return;

	CBaseEntity* localPlayer = (CBaseEntity*)entityList->GetClientEntity(engine->GetLocalPlayer());

	if (!localPlayer)
		return;

	static auto wasLastTimeOnGround{ localPlayer->getFlags() & 1 };

	if (!(localPlayer->getFlags() & 1) && !wasLastTimeOnGround)
		cmd->buttons &= ~CUserCmd::IN_JUMP;

	wasLastTimeOnGround = localPlayer->getFlags() & 1;
}

void* CaptureInterface(const wchar_t* module, const char* name)
{
	if (auto createInterface = reinterpret_cast<void* (*)(const char* pName, int* pReturnCode)>(GetProcAddress(GetModuleHandleW(module), "CreateInterface"))) {
		if (void* found = createInterface(name, nullptr)) {
			return found;
		}
	}
}