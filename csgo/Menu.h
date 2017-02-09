#pragma once

#include "Loader.h"

enum { 
	MENU_VISUAL,
	MENU_AIMBOT, 
	MENU_TRIGGERBOT 
};

struct Config {
	// Visual
	bool espBox;
	bool espSkeleton;
	bool espAllies;
	bool espShowLastPosition;
	float espLastPosition[3];

	// Aimbot
	bool aimbotIsActivated;
	DWORD aimbotKey;
	bool aimbotAutoFire;
	bool silentAim;
	int aimbotWaitingTime;
	int aimbotBoneId;
	bool aimbotAutoZoom;
	bool smoothAimbot;
	float aimbotSmoothSpeed;

	// Triggerbot
	bool triggerbotIsActivated;
	int triggerbotWaitingTime;
};

class Menu {
public:
	static bool save();
	static bool load();
	static void updateMenuState(Loader* loader);
	static bool menuIsOpen();
	static void showMenu(Loader* loader);
	static void showRadarHack(Loader* loader);

	static bool tmpMenuIsOpen;
	static int menuId;
	static Config config;
	static bool waitForAimbotKey;

private:
	static bool m_menuIsOpen;
	static bool m_radarIsActive;
};