#include "AimAssist.h"
#include "Client.h"
#include "ClientEntList.h"
#include "Engine.h"
#include "EngineTrace.h"
#include "BaseCombatWeapon.h"
#include "Math3.h"
#include "boneId.h"
#include "Menu.h"
#include "inButtons.h"

DWORD AimAssist::m_lastShot = 0;
bool AimAssist::m_hasFired = false;
bool AimAssist::m_hasZoomed = false;
Timer AimAssist::m_zoomTimer = Timer();

AimAssist::AimAssist(Loader* loader) {
	m_loader = loader;
}

void AimAssist::triggerBot(UserCmd* userCmd) {
	Client* client = (Client*)m_loader->get(CLIENT);
	ClientEntList* clientEntList = (ClientEntList*)m_loader->get(CLIENTENTLIST);
	Engine* engine = (Engine*)m_loader->get(ENGINE);
	EngineTrace* engineTrace = (EngineTrace*)m_loader->get(ENGINETRACE);

	BaseEntity localPlayer = BaseEntity(m_loader);
	if (!clientEntList->getLocalPlayer(localPlayer)) {
		return;
	}

	if (localPlayer.getHealth() <= 0) {
		return;
	}

	BaseCombatWeapon weapon = BaseCombatWeapon(NULL, m_loader, localPlayer);
	if (!localPlayer.getWeapon(weapon)) {
		return;
	}

	if (!weapon.isGun() || weapon.isEmpty() || weapon.isReloading()) {
		return;
	}

	// Calc the forward vector
	QAngle viewAngle = userCmd->viewangles;
	viewAngle = viewAngle + localPlayer.getPunch() * 2.0f;
	Vector3 forward;
	Math3::angleToForwardVector(viewAngle, forward);
	forward = forward * weapon.getCSWpnData()->m_flRange; 
	Vector3 src = localPlayer.getEyePosition();
	Vector3 dst = src + forward;

	// Check if we target an enemy
	Ray ray;
	GameTrace tr;
	ray.init(src, dst);

	TraceFilter filter;
	filter.pSkip = localPlayer.self();
	engineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	if (tr.m_pEnt != NULL) {
		BaseEntity enemy = BaseEntity(tr.m_pEnt, m_loader);
		if (!enemy.isDormant() && enemy.getHealth() > 0 && enemy.isEnemy()) {
			if (Menu::config.triggerbotWaitingTime > 0) {
				DWORD minDelay = m_lastShot + Menu::config.triggerbotWaitingTime;
				if (GetTickCount() < minDelay) {
					return;
				}
				m_lastShot = GetTickCount();
			}

			userCmd->buttons |= IN_ATTACK;
			m_hasFired = true;
		}
	}
}


void AimAssist::aimBot(UserCmd* userCmd) {
	Client* client = (Client*)m_loader->get(CLIENT);
	ClientEntList* clientEntList = (ClientEntList*)m_loader->get(CLIENTENTLIST);
	Engine* engine = (Engine*)m_loader->get(ENGINE);
	EngineTrace* engineTrace = (EngineTrace*)m_loader->get(ENGINETRACE);

	BaseEntity localPlayer = BaseEntity(m_loader);
	if (!clientEntList->getLocalPlayer(localPlayer)) {
		return;
	}

	if (localPlayer.getHealth() <= 0) {
		return;
	}

	BaseCombatWeapon weapon = BaseCombatWeapon(NULL, m_loader, localPlayer);
	if (!localPlayer.getWeapon(weapon)) {
		return;
	}

	if (!weapon.isGun() || weapon.isEmpty() || weapon.isReloading()) {
		if (weapon.isSniperRifle() && weapon.zoomLevel() > 0) {
			m_zoomTimer.start(400);
		}

		return;
	}

	BaseEntity entity = BaseEntity(m_loader);
	float lowestDistance = -1;
	Vector3 vec;
	float distance;
	PVOID bestEnemy = NULL;

	// Search the best enemy
	for (int i = 0; i < clientEntList->getHighestEntityIndex(); i++) {
		if (!clientEntList->getClientEntity(i, entity)) {
			continue;
		}
		if (entity.getHealth() <= 0) {
			continue;
		}
		if (entity.isEnemy() && entity.isVisible(Terrorist::head_0)) {
			vec = entity.getEyePosition() - localPlayer.getEyePosition();
			distance = vec.lengthSquare();

			if (lowestDistance == -1 || distance < lowestDistance) {
				lowestDistance = distance;
				bestEnemy = entity.self();
			}
		}
	}

	if (bestEnemy == NULL) {
		return;
	}
	BaseEntity enemy = BaseEntity(bestEnemy, m_loader);

	// Calc the angle
	Vector3 src = localPlayer.getEyePosition();
	Vector3 dst = enemy.getBonePosition(Menu::config.aimbotBoneId);
	QAngle angle = Math3::angleBtwVectors(src, dst);
	angle = angle - localPlayer.getPunch() * 2.0f;

	// Smooth aim bot
	QAngle delta;
	if (Menu::config.smoothAimbot) {
		delta = angle - userCmd->viewangles;
		delta.clamp();

		if (!delta.isNull()) {
			angle = userCmd->viewangles + delta*Menu::config.aimbotSmoothSpeed;
		}
	}

	// Set the angle
	angle.clamp();
	userCmd->viewangles = angle;
	if (!Menu::config.silentAim) {
		engine->setViewAngles(angle);
	}

	if (Menu::config.smoothAimbot && !delta.isNull()) {
		return;
	}

	if (Menu::config.aimbotAutoFire) {
		if (Menu::config.aimbotWaitingTime > 0) {
			DWORD minDelay = m_lastShot + Menu::config.aimbotWaitingTime;
			if (GetTickCount() < minDelay) {
				return;
			}
			m_lastShot = GetTickCount();
		}

		if (Menu::config.aimbotAutoZoom && weapon.isSniperRifle() && weapon.zoomLevel() == 0) {
			userCmd->buttons |= IN_ATTACK2;
			m_hasZoomed = true;
			return;
		}

		userCmd->buttons |= IN_ATTACK;
		m_hasFired = true;
	}
}

bool AimAssist::updateFireState(UserCmd* userCmd) {
	if (userCmd->command_number == 0) {
		return false;
	}

	if (m_hasZoomed) {
		m_hasZoomed = false;
		userCmd->buttons &= ~IN_ATTACK2;
		m_zoomTimer.start(250);
		return false;
	}
	if (Menu::config.aimbotAutoZoom && !m_zoomTimer.isElapsed()) {
		return false;
	}
	if (m_hasFired) {
		m_hasFired = false;
		userCmd->buttons &= ~IN_ATTACK;
		return false;
	}

	return true;
}