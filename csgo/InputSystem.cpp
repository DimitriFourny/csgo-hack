#include "InputSystem.h"
#include "vmtIndexes.h"

void InputSystem::getCursorPosition(int* x, int* y) {
	typedef void(__thiscall *GetCursorPositionFn)(PVOID pThis, int* x, int* y);
	GetCursorPositionFn oGetCursorPosition = (GetCursorPositionFn)m_vtable->GetMethodAddress(VMT_GETCURSORPOSITION);
	oGetCursorPosition(self(), x, y);
}