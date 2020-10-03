#pragma once

#include <memory>
#include "Interfaces.h"
#include "CUserCmd.h"

typedef void(__thiscall* createMove_t)(ClientMode*, float inputSampleTime, CUserCmd* cmd);

namespace Hooks
{
	inline uintptr_t* pNewPanelTable, * pOldPanelTable;
	bool hook();
	void unHook();
	static void __stdcall createMove(float inputSampleTime, CUserCmd* cmd);
};
