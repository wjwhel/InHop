#include "Hooks.h"
#include "CBaseEntity.h"

namespace Hooks
{
	bool hook()
	{
		// https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/game/client/iclientmode.h#L89
		// 21 is the index of virtual function CreateMove in IClientMode, count starting at 0 from start of IClientMode class to CreateMove function.
		// Valve sdk is outdated for some things so current index is actually 24 for CreateMove
		addr = *reinterpret_cast<uintptr_t**>(Interfaces::clientMode) + 24;

		DWORD oldProtection;
		VirtualProtect(addr, sizeof(addr), PAGE_EXECUTE_READWRITE, &oldProtection);

		origCreateMove = reinterpret_cast<createMove_t>(*addr);

		*addr = uintptr_t(createMove);

		VirtualProtect(addr, sizeof(addr), oldProtection, 0);
		return true;
	}

	void unHook()
	{
		DWORD oldProtection;
		VirtualProtect(addr, sizeof(addr), PAGE_EXECUTE_READWRITE, &oldProtection);

		*addr = uintptr_t(origCreateMove);

		VirtualProtect(addr, sizeof(addr), oldProtection, 0);
	}

	static bool __stdcall createMove(float inputSampleTime, CUserCmd* cmd)
	{
		origCreateMove(Interfaces::clientMode, inputSampleTime, cmd);

		if (!cmd->commandNumber)
			return true;

		CBaseEntity* localPlayer = (CBaseEntity*)Interfaces::entityList->GetClientEntity(Interfaces::engine->GetLocalPlayer());

		if (!localPlayer)
			return true;

		if (!(localPlayer->getFlags() & 1))
			cmd->buttons &= ~CUserCmd::IN_JUMP;

		return true;
	}
}


