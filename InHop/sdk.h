#pragma once

#include "IClientEntityList.h"
#include "IVEngineClient.h"
#include "CUserCmd.h"

class CBaseEntity
{
public:

	bool GetDormant()
	{
		return *(bool*)(this + 0xE9);
	}

	int GetTeam()
	{
		return *(int*)(this + 0xF0);
	}

	int GetHealth()
	{
		return *(int*)(this + 0xFC);
	}

	int GetFlags()
	{
		return *(int*)(this + 0x104);
	}
};
