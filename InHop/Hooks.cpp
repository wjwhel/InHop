#include "Hooks.h"
#include "CBaseEntity.h"

namespace Hooks
{
	bool hook()
	{
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

	static void __stdcall createMove(float inputSampleTime, CUserCmd* cmd)
	{
		origCreateMove(Interfaces::clientMode, inputSampleTime, cmd);

		if (!cmd->commandNumber)
			return;

		CBaseEntity* localPlayer = (CBaseEntity*)Interfaces::entityList->GetClientEntity(Interfaces::engine->GetLocalPlayer());

		if (!localPlayer)
			return;

		static auto wasLastTimeOnGround{ localPlayer->getFlags() & 1 };

		if (!(localPlayer->getFlags() & 1) && !wasLastTimeOnGround)
			cmd->buttons &= ~CUserCmd::IN_JUMP;

		wasLastTimeOnGround = localPlayer->getFlags() & 1;
	}
}

