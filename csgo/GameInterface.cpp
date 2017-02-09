#include "GameInterface.h"

GameInterface::GameInterface(PVOID pObject, Loader* loader) {
	m_this = pObject;
	m_vtable = new VTableHook((PDWORD*)m_this);
	m_loader = loader;
}

GameInterface::~GameInterface() {
	delete m_vtable;
}

PVOID GameInterface::self() {
	return m_this;
}

void GameInterface::setSelf(PVOID pObject) {
	m_this = pObject;
	delete m_vtable;
	m_vtable = new VTableHook((PDWORD*)m_this);
}

Loader* GameInterface::getLoader() {
	return m_loader;
}