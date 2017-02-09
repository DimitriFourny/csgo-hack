#pragma once

#include <stdio.h>
#include "GameInterface.h"
#include "vmtIndexes.h"

struct RecvProp;
struct RecvTable {
	RecvProp* props;
	int size;
	void* decoder;
	char* netTableName;
	bool initialized;
	bool inMainList;
};

struct RecvProp {
	char* varName;
	int recvType;
	int flags;
	int stringBufferSize;
	bool insideArray;
	const void* extraData;
	RecvProp* arrayProp;
	void* arrayLengthProxy;
	void* proxyFn;
	void* dataTableProxyFn;
	RecvTable* dataTable;
	int offset;
	int elementStride;
	int nbElements;
	const char* parentArrayPropName;
};

struct ClientClass {
	void* createFn;
	void* createEventFn;
	char* networkName;
	RecvTable* recvTable;
	ClientClass* next;
	int classID;
};


class Client : public GameInterface { 
public:
	using GameInterface::GameInterface; 

	int netvarOffset(char* tableName, char* memberName);
	void dumpNetvarOffset();
	PVOID getClientMode();

private:
	ClientClass* getAllClasses();
	int netvarOffset(RecvTable* recvTable, char* memberName);
	int dumpNetvarOffset(RecvTable* recvTable, FILE* file);
};