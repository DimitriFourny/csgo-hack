#include "Loader.h"
#include "Client.h"
#include "ClientEntList.h"
#include "Surface.h"
#include "Vpnanel.h"
#include "Engine.h"

#include <psapi.h>

typedef PVOID(*CreateInterfaceFn)(const char* szName, int iReturn);
IDirect3DDevice9* Loader::m_d3d9Device = NULL;

Loader::Loader() {
	m_interfaces.resize(NB_INTERFACES);
	m_interfaces[CLIENT] = new Client(GetInterface(L"client.dll", "VClient018"), this);
	m_interfaces[CLIENTENTLIST] = new ClientEntList(GetInterface(L"client.dll", "VClientEntityList003"), this);
	m_interfaces[SURFACE] = new Surface(GetInterface(L"vguimatsurface.dll", "VGUI_Surface031"), this);
	m_interfaces[VPANEL] = new Vpanel(GetInterface(L"vgui2.dll", "VGUI_Panel009"), this);
	m_interfaces[ENGINE] = new Engine(GetInterface(L"engine.dll", "VEngineClient014"), this);
	m_interfaces[ENGINETRACE] = new Engine(GetInterface(L"engine.dll", "EngineTraceClient004"), this);
	m_interfaces[INPUTSYSTEM] = new Engine(GetInterface(L"inputsystem.dll", "InputSystemVersion001"), this);

	for (size_t i = 0; i < m_interfaces.size(); i++) {
		if (m_interfaces[i] == NULL) {
			OutputDebugString(L"One interface is NULL");
		}
	}
}

Loader::~Loader() {
	for (size_t i = 0; i < m_interfaces.size(); i++) {
		delete m_interfaces[i];
	}
}

GameInterface* Loader::get(InterfaceType type) {
	return m_interfaces.at(type);
}

PVOID Loader::GetInterface(wchar_t* moduleName, char* interfaceName) {
	HMODULE hModule = GetModuleHandle(moduleName);
	if (hModule == NULL) {
		OutputDebugString(L"Failed to found the following module: ");
		OutputDebugString(moduleName);
		return NULL;
	}

	CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress(hModule, "CreateInterface");
	if (CreateInterface == NULL) {
		OutputDebugString(L"Failed to load CreateInterface from module: ");
		OutputDebugString(moduleName);
		return NULL;
	}

	return CreateInterface(interfaceName, 0);
}


#define INRANGE(x,a,b) (x >= a && x <= b) 
#define getBits(x) (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte(x) (getBits(x[0]) << 4 | getBits(x[1]))
DWORD Loader::findPattern(std::string module, std::string pattern) {
	HMODULE hModule = GetModuleHandleA(module.c_str());
	if (hModule == NULL) {
		return 0;
	}

	MODULEINFO modInfo;
	BOOL success = GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(modInfo));
	if (!success) {
		return 0;
	}

	char* data = (char*)hModule; 
	unsigned int size = modInfo.SizeOfImage;
	unsigned int i = 0;
	unsigned int indexPattern;
	unsigned int numPattern;
	unsigned int patternFound;
	DWORD firstOccurence = 0;
	char byte;

	while (i < size - pattern.size()) {
		patternFound = false;
		indexPattern = 0;
		numPattern = 0;

		while (indexPattern < pattern.size()) {
			if (pattern[indexPattern] == ' ') {
				indexPattern++;
				continue;
			}
			if (pattern[indexPattern] == '?') {
				indexPattern++;
				i++;
				continue;
			}

			byte = getByte((&pattern[indexPattern]));

			if (data[i+numPattern] != byte) {
				break;
			} else {
				if (numPattern == 0) {
					firstOccurence = (DWORD) data + i;
				}
			}
			
			indexPattern += 2;
			numPattern++;

			if (indexPattern >= pattern.size()) {
				patternFound = true;
				break;
			}
		}

		if (patternFound) {
			return firstOccurence;
		}

		i++;
	}

	return 0;
}

IDirect3DDevice9* Loader::getD3d9Device() {
	if (m_d3d9Device == NULL) {
		/*
		0:050> u shaderapidx9+0x2da0e:
			0f47da0e a1608d4e0f      mov     eax,dword ptr [shaderapidx9!CreateInterface+0x5b370 (0f4e8d60)]
			0f47da13 50              push    eax
			0f47da14 8b08            mov     ecx,dword ptr [eax]
			0f47da16 ff510c          call    dword ptr [ecx+0Ch]
		*/
		DWORD pattern = findPattern("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C");
		PDWORD pD3d9Device = *(PDWORD*)(pattern + 1);
		m_d3d9Device = (IDirect3DDevice9*) *pD3d9Device;
	}

	return m_d3d9Device;
}