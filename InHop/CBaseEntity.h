#pragma once

class CBaseEntity
{
public:
	int getFlags()
	{
		return *(int*)(this + 0x104); // Current m_fFlags = 0x104
	}
};
