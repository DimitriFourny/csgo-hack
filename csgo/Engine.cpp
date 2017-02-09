#include "Engine.h"


bool Engine::screenTransform(const Vector3& world, Vector2& screen) {
	float w;
	Vmatrix& worldToScreen = worldToScreenMatrix();

	screen.x = worldToScreen[0][0] * world.x + worldToScreen[0][1] * world.y + worldToScreen[0][2] * world.z + worldToScreen[0][3];
	screen.y = worldToScreen[1][0] * world.x + worldToScreen[1][1] * world.y + worldToScreen[1][2] * world.z + worldToScreen[1][3];
	w = worldToScreen[3][0] * world.x + worldToScreen[3][1] * world.y + worldToScreen[3][2] * world.z + worldToScreen[3][3];

	bool behind = false;

	if (w < 0.001f) {
		behind = true;
		screen.x *= 100000;
		screen.y *= 100000;
	}
	else {
		behind = false;
		float invw = 1.0f / w;
		screen.x *= invw;
		screen.y *= invw;
	}

	return behind;
}

bool Engine::worldToScreen(const Vector3& origin, Vector2& screen) {
	if (!screenTransform(origin, screen)) {
		int screenWidth = 0, screenHeight = 0;
		getScreenSize(screenWidth, screenHeight);

		float x = (float)screenWidth / 2.0f;
		float y = (float)screenHeight / 2.0f;
		x += 0.5f * screen.x * screenWidth + 0.5f;
		y -= 0.5f * screen.y * screenHeight + 0.5f;
		screen.x = x;
		screen.y = y;

		return true;
	}

	return false;
}

void Engine::getScreenSize(int& width, int& height) {
	typedef void(__thiscall *GetScreenSizeFn)(PVOID pThis, int& width, int& height);
	GetScreenSizeFn oGetScreenSize = (GetScreenSizeFn)m_vtable->GetMethodAddress(VMT_GETSCREENSIZE);
	oGetScreenSize(self(), width, height);
}

int Engine::getLocalPlayer() {
	typedef int(__thiscall *GetLocalPlayerFn)(PVOID pThis);
	GetLocalPlayerFn oGetLocalPlayer = (GetLocalPlayerFn)m_vtable->GetMethodAddress(VMT_GETLOCALPLAYER);
	return oGetLocalPlayer(self());
}

struct Vmatrix& Engine::worldToScreenMatrix() {
	typedef struct Vmatrix&(__thiscall *WorldToScreenMatrixFn)(PVOID pThis);
	WorldToScreenMatrixFn oWorldToScreenMatrix = (WorldToScreenMatrixFn)m_vtable->GetMethodAddress(VMT_WORLDTOSCREENMATRIX);
	return oWorldToScreenMatrix(self());
}

bool Engine::getPlayerInfo(int entNum, PlayerInfo* playerInfo) {
	typedef bool(__thiscall *GetPlayerInfoFn)(PVOID pThis, int entNum, PlayerInfo* playerInfo);
	GetPlayerInfoFn oGetPlayerInfo = (GetPlayerInfoFn)m_vtable->GetMethodAddress(VMT_GETPLAYERINFO);
	return oGetPlayerInfo(self(), entNum, playerInfo);
}

void Engine::setViewAngles(const QAngle& angle) {
	typedef void(__thiscall *SetViewAnglesFn)(PVOID pThis, const QAngle& angle);
	SetViewAnglesFn oSetViewAngles = (SetViewAnglesFn)m_vtable->GetMethodAddress(VMT_SETVIEWANGLES);
	return oSetViewAngles(self(), angle);
}

void Engine::clientCmd(const char* cmd) {
	typedef void(__thiscall *ClientCmd_UnrestrictedFn)(PVOID pThis, const char* cmd, bool unk0);
	ClientCmd_UnrestrictedFn oClientCmd_Unrestricted = (ClientCmd_UnrestrictedFn)m_vtable->GetMethodAddress(VMT_CLIENTCMDUNRESTRICTED);
	oClientCmd_Unrestricted(self(), cmd, false);
}