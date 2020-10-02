#pragma once

class IClientEntityList
{
public:
	virtual void* GetClientNetworkable(int entnum) = 0;
	virtual void* GetClientNetworkableFromHandle(int hEnt) = 0;
	virtual void* GetClientUnknownFromHandle(int hEnt) = 0;
	virtual void* GetClientEntity(int entnum) = 0;
	virtual void* GetClientEntityFromHandle(int hEnt) = 0;
	virtual int					NumberOfEntities(bool bIncludeNonNetworkable) = 0;
	virtual int					GetHighestEntityIndex(void) = 0;
	virtual void				SetMaxEntities(int maxents) = 0;
	virtual int					GetMaxEntities() = 0;
};