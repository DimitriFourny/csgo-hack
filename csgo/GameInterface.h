#pragma once

#include <Windows.h>
#include "VTableHook.h"

class Loader;

class GameInterface {
public:
	GameInterface(Loader* loader) : GameInterface(NULL, loader) {};
	GameInterface(PVOID pObject, Loader* loader);
	~GameInterface();
	PVOID self();
	void setSelf(PVOID pObject);
	Loader* getLoader();

protected:
	PVOID m_this;
	VTableHook* m_vtable;
	Loader* m_loader;
};
