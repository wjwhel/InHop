#pragma once

#include <Windows.h>
#include "IClientEntityList.h"
#include "IVEngineClient.h"

class IBaseClientDLL;
class IClientMode;

namespace Interfaces
{
	inline IBaseClientDLL* client			= nullptr;
	inline IClientEntityList* entityList	= nullptr;
	inline IVEngineClient* engine			= nullptr;
	inline IClientMode* clientMode			= nullptr;

	bool init();
	static void* CaptureInterface(const wchar_t* module, const char* name);
};
