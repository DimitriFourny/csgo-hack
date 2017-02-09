#pragma once

#include "GameInterface.h"

class InputSystem : public GameInterface {
public:
	using GameInterface::GameInterface;
	void getCursorPosition(int* x, int* y);
};