#pragma once

#include "GameInterface.h"
#include "vmtIndexes.h"


class Vpanel : public GameInterface { 
public:
	using GameInterface::GameInterface; 

	char* getName(int index);
};