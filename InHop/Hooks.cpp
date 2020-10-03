#include "Hooks.h"
#include "CBaseEntity.h"

namespace Hooks
{
	bool hook()
	{
		const auto fnEstimateTableLength = [](uintptr_t* pTable)
		{
			auto sReturn = std::size_t(0u);
			MEMORY_BASIC_INFORMATION mbiTable{ };

			while (NULL != VirtualQuery(reinterpret_cast<LPCVOID>(pTable[sReturn]), &mbiTable, sizeof mbiTable) &&
				mbiTable.BaseAddress != nullptr &&
				mbiTable.Type != NULL &&
				mbiTable.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY) &&
				!(mbiTable.Protect & (PAGE_GUARD | PAGE_NOACCESS)))
				sReturn++;
			return sReturn;
		};

		pOldPanelTable = *reinterpret_cast<uintptr_t**>(Interfaces::clientMode);

		if (pOldPanelTable == nullptr) {
			return false;
		}

		const auto length = fnEstimateTableLength(pOldPanelTable);

		if (length == NULL) {
			return false;
		}

		pNewPanelTable = new uintptr_t[length];

		memcpy(pNewPanelTable, pOldPanelTable, length * sizeof(uintptr_t));

		*reinterpret_cast<uintptr_t**>(Interfaces::clientMode) = pNewPanelTable;

		pNewPanelTable[24] = uintptr_t(createMove);

		return true;
	}

	void unHook()
	{
		*reinterpret_cast<uintptr_t**>(Interfaces::clientMode) = pOldPanelTable;
		delete pNewPanelTable;
	}

	static void __stdcall createMove(float inputSampleTime, CUserCmd* cmd)
	{
		static auto fnOriginal = createMove_t(pOldPanelTable[24]);

		fnOriginal(Interfaces::clientMode, inputSampleTime, cmd);

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


