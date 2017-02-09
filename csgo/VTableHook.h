#pragma once

#include <Windows.h>


class VTableHook {

public:
	VTableHook(PDWORD* pClassBase);
	~VTableHook();
	DWORD VTableHook::GetMethodAddress(unsigned int index);
	unsigned int getVTableSize();
	DWORD VTableHook::HookMethod(DWORD dwNewFunc, unsigned int index);

private:
	void hookVTable();
	PDWORD*	m_pClassBase;
	PDWORD	m_pVTable;
	PDWORD	m_pOriginalVTable;
};
