#pragma once

#include <d3d9.h>
#include "UserCmd.h"


class Hooks {
public:
	static void setupHooks();
	static void __stdcall PaintTraverse(unsigned int vguiPanel, bool forceRepaint, bool allowForce);
	static bool __stdcall CreateMove(float inputSampleTime, UserCmd* userCmd);
	static long __stdcall EndScene(IDirect3DDevice9* device);
	static LRESULT __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static QAngle m_viewAngles;
};