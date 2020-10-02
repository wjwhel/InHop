#pragma once

#include "IClientEntityList.h"
#include "IVEngineClient.h"
#include "CUserCmd.h"

class IBaseClientDLL;
class ClientMode;

class CBaseEntity
{
public:
	int getFlags()
	{
		return *(int*)(this + 0x104); // Current m_fFlags = 0x104
	}
};
