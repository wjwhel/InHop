#include "Interfaces.h"

namespace Interfaces
{
	// Once you have a interface you can call its functions without knowing the implementation
	// https://github.com/ValveSoftware/source-sdk-2013/blob/0d8dceea4310fde5706b3ce1c70609d72a38efdf/sp/src/public/icliententitylist.h#L28
	// IClientEntityList interface that i capture below - after captured we cast it to a copy of the class IClientEntityList.h to call the functions.
	bool init()
	{
		client		= reinterpret_cast<IBaseClientDLL*>(CaptureInterface(L"client.dll", "VClient018"));

		// VClientEntityList003 version of interface can be seen here https://github.com/ValveSoftware/source-sdk-2013/blob/0d8dceea4310fde5706b3ce1c70609d72a38efdf/sp/src/public/icliententitylist.h#L54
		// But can also easily be found in IDA if updated by opening csgo's client.dll.
		entityList	= reinterpret_cast<IClientEntityList*>(CaptureInterface(L"client.dll", "VClientEntityList003"));
		engine		= reinterpret_cast<IVEngineClient*>(CaptureInterface(L"engine.dll", "VEngineClient014"));

		// ClientMode, this is where we are going to hook CreateMove.
		// To get a pointer to clientmode use global pointer 'g_pClientMode' here in HudProcessInput function https://github.com/ValveSoftware/source-sdk-2013/blob/0d8dceea4310fde5706b3ce1c70609d72a38efdf/sp/src/game/client/cdll_client_int.cpp#L1248
		// HudProcessInput is a virtual function in IBaseClientDLL here https://github.com/ValveSoftware/source-sdk-2013/blob/0d8dceea4310fde5706b3ce1c70609d72a38efdf/sp/src/game/client/cdll_client_int.cpp#L614
		// HudProcessInput virtual function is at index 10 of IBaseClientDLL.
		// + 5 or 0x5 is the offset to the g_pClientMode pointer.
		clientMode	= **reinterpret_cast<IClientMode***>((*reinterpret_cast<uintptr_t**>(client))[10] + 5);

		return client != nullptr && entityList != nullptr && engine != nullptr && clientMode != nullptr;
	}

	// CreateInterface, exported function - https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/tier1/interface.cpp#L68
	// CreateInterface is a exported function* in csgo's client.dll, engine.dll and other csgo DLL's which returns the address of an interface***.

	// * Export functions are functions that a module** exposes to other modules
	// ** Pieces of code eg. DLL's 
	// *** abstract function. abtract class like java. declares prototypes but does not define them
	static void* CaptureInterface(const wchar_t* module, const char* name)
	{
		if (auto createInterface = reinterpret_cast<void* (*)(const char* pName, int* pReturnCode)>(GetProcAddress(GetModuleHandleW(module), "CreateInterface"))) {
			if (void* foundInterface = createInterface(name, nullptr)) {
				return foundInterface;
			}
		}
	}
}