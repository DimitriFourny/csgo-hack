#pragma once

#include <Windows.h>
#include <vector>
#include "GameInterface.h"
#include <d3d9.h>

enum InterfaceType {
	CLIENT,
	CLIENTENTLIST,
	SURFACE,
	VPANEL,
	ENGINE,
	ENGINETRACE,
	INPUTSYSTEM, 
	NB_INTERFACES
};

class Loader {
public:
	Loader();
	~Loader();
	GameInterface* get(InterfaceType type);
	DWORD findPattern(std::string module, std::string pattern);
	IDirect3DDevice9* getD3d9Device();

private:
	PVOID GetInterface(wchar_t* moduleName, char* interfaceName);
	std::vector<GameInterface*> m_interfaces;
	static IDirect3DDevice9* m_d3d9Device;
};