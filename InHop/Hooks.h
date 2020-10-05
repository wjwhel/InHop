#pragma once

#include <memory>
#include "Interfaces.h"
#include "CUserCmd.h"

typedef bool(__thiscall* createMove_t)(IClientMode*, float inputSampleTime, CUserCmd* cmd);

namespace Hooks
{
	inline uintptr_t *addr;
	inline createMove_t origCreateMove;
	bool hook();
	void unHook();
	static bool __stdcall createMove(float inputSampleTime, CUserCmd* cmd);
};
