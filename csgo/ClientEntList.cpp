#include "ClientEntList.h"

bool ClientEntList::getClientEntity(int index, OUT BaseEntity& entity) {
	typedef void*(__thiscall *GetClientEntityFn)(PVOID pThis, int index);
	GetClientEntityFn oGetClientEntity = (GetClientEntityFn) m_vtable->GetMethodAddress(VMT_GETCLIENTENTITY);
	PVOID pEntity = oGetClientEntity(self(), index);
	if (pEntity == NULL) {
		return false;
	}

	entity.setSelf(pEntity);
	return true;
}

bool ClientEntList::getClientEntityFromHandle(HANDLE handle, OUT BaseEntity& entity) {
	typedef void*(__thiscall *GetClientEntityFromHandleFn)(PVOID pThis, HANDLE handle);
	GetClientEntityFromHandleFn oGetClientEntityFromHandle = (GetClientEntityFromHandleFn)m_vtable->GetMethodAddress(VMT_GETCLIENTENTITYFROMHANDLE);
	PVOID pEntity = oGetClientEntityFromHandle(self(), handle);
	if (pEntity == NULL) {
		return false;
	}

	entity.setSelf(pEntity);
	return true;
}

int ClientEntList::numberOfEntities(bool includeNonNetworkable) {
	typedef int(__thiscall *NumberOfEntitiesFn)(PVOID pThis, bool includeNonNetworkable);
	NumberOfEntitiesFn oNumberOfEntities = (NumberOfEntitiesFn)m_vtable->GetMethodAddress(VMT_NUMBEROFENTITIES);
	return oNumberOfEntities(self(), includeNonNetworkable);
}

int ClientEntList::getHighestEntityIndex() {
	typedef int(__thiscall *GetHighestEntityIndexFn)(PVOID pThis);
	GetHighestEntityIndexFn oGetHighestEntityIndex = (GetHighestEntityIndexFn)m_vtable->GetMethodAddress(VMT_GETHIGHESTENTITYINDEX);
	return oGetHighestEntityIndex(self());
}

bool ClientEntList::getLocalPlayer(OUT BaseEntity& entity) {
	Engine* engine = (Engine*) m_loader->get(ENGINE);
	return getClientEntity(engine->getLocalPlayer(), entity);
}