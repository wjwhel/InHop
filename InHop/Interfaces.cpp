#include "Interfaces.h"

namespace Interfaces
{
	bool init()
	{
		client		= reinterpret_cast<IBaseClientDLL*>(CaptureInterface(L"client.dll", "VClient018"));
		entityList	= reinterpret_cast<IClientEntityList*>(CaptureInterface(L"client.dll", "VClientEntityList003"));
		engine		= reinterpret_cast<IVEngineClient*>(CaptureInterface(L"engine.dll", "VEngineClient014"));
		clientMode	= **reinterpret_cast<IClientMode***>((*reinterpret_cast<uintptr_t**>(client))[10] + 5);

		return client != nullptr && entityList != nullptr && engine != nullptr && clientMode != nullptr;
	}

	// CreateInterface exported function which returns address of interface.
	static void* CaptureInterface(const wchar_t* module, const char* name)
	{
		auto createInterface = reinterpret_cast<void* (__cdecl*)(const char* pName, int* pReturnCode)>(GetProcAddress(GetModuleHandleW(module), "CreateInterface"));
		void* Interface = createInterface(name, nullptr);

		return Interface;
	}
}