#include "VTableHook.h"

VTableHook::VTableHook(PDWORD* pClassBase) {
	m_pClassBase = pClassBase;
	m_pVTable = NULL;
	m_pOriginalVTable = NULL;
}

VTableHook::~VTableHook() {
	if (m_pVTable != NULL) {
		VirtualFree(m_pVTable, 0, MEM_RELEASE);
	}
}

DWORD VTableHook::GetMethodAddress(unsigned int index) {
	if (m_pOriginalVTable == NULL) {
		m_pOriginalVTable = *m_pClassBase;
	}

	return m_pOriginalVTable[index];
}

unsigned int VTableHook::getVTableSize() {
	unsigned int size = 0;
	DWORD vtable = *(PDWORD)m_pClassBase;

	DWORD function;
	while (true) {
		function = *(PDWORD)(vtable + sizeof(DWORD)*size);
		if (function <= 0x1000) {
			break;
		}
		size++;
	}

	return size;
}

void VTableHook::hookVTable() {
	DWORD dwOldProtect, dwOldProtect2;

	if (m_pVTable == NULL) {
		m_pOriginalVTable = *m_pClassBase;
		unsigned int vtableSize = getVTableSize();

		// Copy the actual vtable
		m_pVTable = (PDWORD) VirtualAlloc(NULL, sizeof(DWORD)*vtableSize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
		memcpy(m_pVTable, m_pOriginalVTable, sizeof(DWORD)*vtableSize);

		// Assign the same protection that the real vtable (hello VAC)
		MEMORY_BASIC_INFORMATION memInfo;
		VirtualQuery(m_pOriginalVTable, &memInfo, sizeof(memInfo));
		VirtualProtect(m_pVTable, sizeof(DWORD)*vtableSize, memInfo.Protect, &dwOldProtect);
		
		// Replace the vtable pointer
		VirtualProtect(m_pClassBase, sizeof(DWORD), PAGE_READWRITE, &dwOldProtect);
		*m_pClassBase = m_pVTable;
		VirtualProtect(m_pClassBase, sizeof(DWORD), dwOldProtect, &dwOldProtect2);
	}
}

DWORD VTableHook::HookMethod(DWORD dwNewFunc, unsigned int index) {
	DWORD dwOldProtect, dwOldProtect2;

	hookVTable();

	VirtualProtect(&m_pVTable[index], sizeof(DWORD), PAGE_READWRITE, &dwOldProtect);
	m_pVTable[index] = dwNewFunc;
	VirtualProtect(&m_pVTable[index], sizeof(DWORD), dwOldProtect, &dwOldProtect2);

	return m_pVTable[index];
}