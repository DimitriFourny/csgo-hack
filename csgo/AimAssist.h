#pragma once

#include "Loader.h"
#include "UserCmd.h"
#include "Timer.h"

class AimAssist {
public:
	AimAssist(Loader* loader);
	void triggerBot(UserCmd* userCmd);
	void aimBot(UserCmd* userCmd);
	bool updateFireState(UserCmd* userCmd);

private:
	Loader* m_loader;
	static DWORD m_lastShot;
	static bool m_hasFired;
	static bool m_hasZoomed;
	static Timer m_zoomTimer;
};