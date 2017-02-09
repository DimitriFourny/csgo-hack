#pragma once

#include "GameInterface.h"
#include "vmtIndexes.h"
#include "Engine.h"
#include "Loader.h"
#include "BaseEntity.h"

class ClientEntList : public GameInterface { 
public:
	using GameInterface::GameInterface;

	bool getClientEntity(int index, OUT BaseEntity& entity);
	bool getClientEntityFromHandle(HANDLE handle, OUT BaseEntity& entity);
	int numberOfEntities(bool includeNonNetworkable);
	int getHighestEntityIndex();
	bool getLocalPlayer(OUT BaseEntity& entity);
};