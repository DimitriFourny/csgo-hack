#include "Hooks.h"
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <sstream>
#include <d3d9.h>

#include "VTableHook.h"
#include "Loader.h"
#include "Vpnanel.h"
#include "ClientEntList.h"
#include "Surface.h"
#include "Client.h"
#include "boneId.h"
#include "UserCmd.h"
#include "BaseCombatWeapon.h"
#include "InputSystem.h"
#include "imgui\imgui.h"
#include "imgui\imgui_impl_dx9.h"
#include "Esp.h"
#include "AimAssist.h"
#include "Menu.h"
#include "inButtons.h"
#include "Math3.h"

typedef void(__thiscall *PaintTraverseFn)(PVOID pThis, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
PaintTraverseFn oPaintTraverse;
typedef bool(__thiscall *CreateMoveFn)(PVOID pThis, float inputSampleTime, UserCmd* cmd);
CreateMoveFn oCreateMove;
typedef long(__stdcall *EndSceneFn)(IDirect3DDevice9* device);
EndSceneFn oEndScene;
HFONT fontConsolas = NULL;
int g_gamePanel = 0;
Loader* g_loader;

bool d3dIsInit = false;
HWND valveWindow = NULL;
WNDPROC oWndProc = NULL;

QAngle Hooks::m_viewAngles = QAngle(0,0,0);

void Hooks::setupHooks() {
	g_loader = new Loader();
	Surface* surface = (Surface*)g_loader->get(SURFACE);
	Vpanel* vpanel = (Vpanel*)g_loader->get(VPANEL);
	Client* client = (Client*)g_loader->get(CLIENT);

	//client->dumpNetvarOffset();

	// Load font
	fontConsolas = surface->createFont();
	surface->setFontGlyphSet(fontConsolas, "Consolas", 12, FW_DONTCARE, NULL, NULL, FONTFLAG_OUTLINE, 0, 0);

	// Hook PaintTraverse
	VTableHook paintTraverseHook = VTableHook((DWORD**)vpanel->self());
	oPaintTraverse = (PaintTraverseFn)paintTraverseHook.GetMethodAddress(VMT_PAINTTRAVERSE);
	paintTraverseHook.HookMethod((DWORD)PaintTraverse, VMT_PAINTTRAVERSE);
	
	// Hook CreateMove
	PVOID clientMode = client->getClientMode();
	VTableHook createMoveHook = VTableHook((DWORD**)clientMode);
	oCreateMove = (CreateMoveFn)createMoveHook.GetMethodAddress(VMT_CREATEMOVE);
	createMoveHook.HookMethod((DWORD)CreateMove, VMT_CREATEMOVE);

	// Hook WndProc
	while (valveWindow == NULL) {
		valveWindow = FindWindow(L"Valve001", NULL);
		Sleep(10);
	}
	oWndProc = (WNDPROC)GetWindowLongPtr(valveWindow, GWL_WNDPROC);
	SetWindowLongPtr(valveWindow, GWL_WNDPROC, (LONG_PTR)WndProc);
	
	// Hook EndScene
	IDirect3DDevice9* d3d9Device = g_loader->getD3d9Device();
	VTableHook endSceneHook = VTableHook((DWORD**)d3d9Device);
	oEndScene = (EndSceneFn)endSceneHook.GetMethodAddress(VMT_ENDSCENE);
	endSceneHook.HookMethod((DWORD)EndScene, VMT_ENDSCENE);
	
	// @todo: hook reset for ImGui

	while (true) { 
		/* Infinite loop or it will destroy ours hooks */
		Sleep(100);
	}
}

std::wstring ptrToStr(std::wstring name, PVOID ptr) {
	std::wstring msg = name + L": 0x";
	std::wstringstream stream;
	stream << std::hex << ptr;
	msg += stream.str();
	return msg;
}

void drawDebug(std::wstring msg, int line) {
	Surface* surface = (Surface*)g_loader->get(SURFACE);
	surface->drawSetTextFont(fontConsolas);
	surface->drawSetTextColor(255, 255, 255, 255);
	surface->drawSetTextPos(10, line * 10);
	surface->drawPrintText(msg.c_str(), msg.size());
}

void drawDebug(std::string msg, int line) {
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;
	std::wstring wmsg = converter.from_bytes(msg);
	drawDebug(wmsg, line);
}

void __stdcall Hooks::PaintTraverse(unsigned int vguiPanel, bool forceRepaint, bool allowForce) {
	Vpanel* vpanel = (Vpanel*)g_loader->get(VPANEL);
	ClientEntList* clientEntList = (ClientEntList*)g_loader->get(CLIENTENTLIST);
	Engine* engine = (Engine*)g_loader->get(ENGINE);
	Client* client = (Client*)g_loader->get(CLIENT);

	oPaintTraverse(vpanel->self(), vguiPanel, forceRepaint, allowForce);

	if (g_gamePanel == 0) {
		char* panelName = vpanel->getName(vguiPanel);
		if (panelName[0] != 'M' || panelName[2] != 't') {
			return;
		}

		g_gamePanel = vguiPanel;
	}
	if (vguiPanel != g_gamePanel) {
		return;
	}

	std::wstring msg = L"DEBUG";
	drawDebug(msg, 0);

	BaseEntity localPlayer = BaseEntity(g_loader);
	if (!clientEntList->getLocalPlayer(localPlayer)) {
		return;
	}

	msg = L"Team " + std::to_wstring(localPlayer.getTeamNum());
	drawDebug(msg, 1);
	msg = std::to_wstring(localPlayer.getHealth()) + L" HP";
	drawDebug(msg, 2);
	msg = std::to_wstring(localPlayer.getAmorValue()) + L" Armor";
	drawDebug(msg, 3);
	BaseCombatWeapon weapon = BaseCombatWeapon(NULL, g_loader, localPlayer);
	if (!localPlayer.getWeapon(weapon)) {
		return;
	}
	std::string smsg = std::string("weapon: ") + std::string((char*) &weapon.getCSWpnData()->m_name);
	drawDebug(smsg, 4);
	msg = L"weapon range: " + std::to_wstring(weapon.getCSWpnData()->m_flRange);
	drawDebug(msg, 5);
	msg = L"angle y: " + std::to_wstring(Hooks::m_viewAngles.y);
	drawDebug(msg, 6);

	int line = 0;
	BaseEntity entity = BaseEntity(g_loader);
	Esp esp = Esp(g_loader);

	for (int i = 0; i < clientEntList->getHighestEntityIndex(); i++) {
		if (!clientEntList->getClientEntity(i, entity)) {
			continue;
		}
		if (entity.getHealth() <= 0) {
			continue;
		}
		if (entity.self() == localPlayer.self()) {
			continue;
		}

		if (Menu::config.espAllies || entity.isEnemy()) {
			if (Menu::config.espBox) {
				esp.drawBox(entity);
			} 
			if (Menu::config.espSkeleton) {
				esp.drawSkeleton(entity);
			}
		}
	}
}

bool __stdcall Hooks::CreateMove(float inputSampleTime, UserCmd* userCmd) {
	Client* client = (Client*)g_loader->get(CLIENT);
	ClientEntList* clientEntList = (ClientEntList*)g_loader->get(CLIENTENTLIST);
	Engine* engine = (Engine*)g_loader->get(ENGINE);
	EngineTrace* engineTrace = (EngineTrace*)g_loader->get(ENGINETRACE);

	oCreateMove(client->getClientMode(), inputSampleTime, userCmd);
	m_viewAngles = userCmd->viewangles;

	AimAssist aim = AimAssist(g_loader);
	if (aim.updateFireState(userCmd)) {
		if (Menu::config.aimbotIsActivated || GetKeyState(Menu::config.aimbotKey) & 0x8000) {
			aim.aimBot(userCmd);
		}
		else if (Menu::config.triggerbotIsActivated) {
			aim.triggerBot(userCmd);
		}
	}

	return false;
}

long __stdcall Hooks::EndScene(IDirect3DDevice9* device) {
	if (!d3dIsInit) {
		ImGui_ImplDX9_Init(valveWindow, device);
		d3dIsInit = true;
	} else {
		Menu::showMenu(g_loader);
	}

	return oEndScene(device);
}

LRESULT __stdcall Hooks::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_KEYDOWN) {
		if (wParam == VK_INSERT) {
			Menu::tmpMenuIsOpen = !Menu::tmpMenuIsOpen;
		}
		if (Menu::waitForAimbotKey) {
			Menu::config.aimbotKey = wParam;
			Menu::waitForAimbotKey = false;
		}
	}

	if (d3dIsInit) {
		if (Menu::menuIsOpen() && ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
			return true;
		}
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}