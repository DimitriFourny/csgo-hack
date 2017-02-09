#include "Vpnanel.h"


char* Vpanel::getName(int index) {
	typedef char*(__thiscall *GetNameFn)(PVOID pThis, int index);
	GetNameFn oGameName = (GetNameFn)m_vtable->GetMethodAddress(VMT_GETPANELNAME);
	return oGameName(self(), index);
}