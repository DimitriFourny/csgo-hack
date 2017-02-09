#include "Client.h"

#include <string>
#include <sstream>

int Client::netvarOffset(char* tableName, char* memberName) {
	ClientClass* clientClass = getAllClasses();

	while (clientClass != NULL) {
		if (strcmp(clientClass->recvTable->netTableName, tableName) == 0) {
			return netvarOffset(clientClass->recvTable, memberName);
		}

		clientClass = clientClass->next;
	}

	return 0;
}

int Client::netvarOffset(RecvTable* recvTable, char* memberName) {
	RecvProp* prop;
	RecvTable* child;
	int extraOffset = 0;
	int tmpOffset;

	for (int i = 0; i < recvTable->size; i++) {
		prop = &recvTable->props[i];
		child = prop->dataTable;

		if (child != NULL && child->size > 0) {
			tmpOffset = netvarOffset(child, memberName);
			if (tmpOffset != 0) {
				extraOffset += prop->offset + tmpOffset;
			}
		}

		if (strcmp(prop->varName, memberName) == 0) {
			return prop->offset + extraOffset;
		}
	}

	return extraOffset;
}

void Client::dumpNetvarOffset() {
	FILE* file = NULL;
	errno_t err = fopen_s(&file, "C:\\csgo\\dump.txt", "w+");
	if (err != 0) {
		return;
	}
	
	ClientClass* clientClass = getAllClasses();

	while (clientClass != NULL) {
		dumpNetvarOffset(clientClass->recvTable, file);
		clientClass = clientClass->next;
	}

	fclose(file);
}

int Client::dumpNetvarOffset(RecvTable* recvTable, FILE* file) {
	RecvProp* prop;
	RecvTable* child;
	int extraOffset = 0;
	int tmpOffset;

	for (int i = 0; i < recvTable->size; i++) {
		prop = &recvTable->props[i];
		child = prop->dataTable;

		if (child != NULL && child->size > 0) {
			tmpOffset = dumpNetvarOffset(child, file);
			if (tmpOffset != 0) {
				extraOffset += prop->offset + tmpOffset;
			}
		}

		fprintf(file, "%s :: %s :: 0x%X\n", recvTable->netTableName, prop->varName, prop->offset + extraOffset);
	}

	return extraOffset;
}

ClientClass* Client::getAllClasses() {
	typedef ClientClass*(__thiscall *GetAllClassesFn)(PVOID pThis);
	GetAllClassesFn oGetAllClasses = (GetAllClassesFn)m_vtable->GetMethodAddress(VMT_GETALLCLASSES);
	return oGetAllClasses(self());
}

PVOID Client::getClientMode() {
	DWORD addrHudProcessInput = m_vtable->GetMethodAddress(VMT_HUDPROCESSINPUT);
	if (addrHudProcessInput == NULL) {
		return NULL;
	}

	/*
	0:050> u client+0x22e040:
		1c0ce040 55              push    ebp
		1c0ce041 8bec            mov     ebp,esp
		1c0ce043 8b0dc867d820    mov     ecx,dword ptr [client!CreateInterface+0x4805a18 (20d867c8)]
		1c0ce049 8b01            mov     eax,dword ptr [ecx]
		1c0ce04b 5d              pop     ebp
		1c0ce04c ff6030          jmp     dword ptr [eax+30h]
	0:050> dd client+0x22e040 + 5
		1c4ae045  211667c8
	*/
	PVOID* clientMode = *(PVOID**)(addrHudProcessInput + 5); // clientMode = client!CreateInterface+0x4805a18
	return *clientMode;
}