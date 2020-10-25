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

	static void* CaptureInterface(const wchar_t* module, const char* name)
	{
		if (auto createInterface = reinterpret_cast<void* (*)(const char* pName, int* pReturnCode)>(GetProcAddress(GetModuleHandleW(module), "CreateInterface"))) {
			if (void* foundInterface = createInterface(name, nullptr)) {
				return foundInterface;
			}
		}
	}
}