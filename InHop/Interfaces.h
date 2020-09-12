#pragma once

#include "Windows.h"

class Client;

class Interfaces {

	Client* client = reinterpret_cast<Client*>(CaptureInterface(L"client", "VClient018")); // Client version VClient018 found from dissasembling client_panorama.dll

	// GetModuleHandleW returns a handle to module that contains the function "CreateInterface"
	// GetProcAddress returns the address of the function "CreateInterface"
	static void* CaptureInterface(const wchar_t* module, const char* name)
	{
		if (auto createInterface = reinterpret_cast<void* (*)(const char* pName, int* pReturnCode)>(GetProcAddress(GetModuleHandleW(module), "CreateInterface"))) {
			if (void* found = createInterface(name, nullptr)) {
				return found;
			}
		}
	}
};
