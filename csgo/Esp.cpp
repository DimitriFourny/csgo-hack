#include "Esp.h"
#include "Surface.h"
#include "Engine.h"
#include "boneId.h"
#include "Menu.h"
#include "BaseCombatWeapon.h"
#include <string>
#include <locale>
#include <codecvt>
#include <sstream>

Esp::Esp(Loader* loader) {
	m_loader = loader;
}

void Esp::setEspColor(BaseEntity& entity) {
	Surface* surface = (Surface*)m_loader->get(SURFACE);

	// Allie
	if (!entity.isEnemy()) {
		surface->drawSetColor(0, 0, 255, 255);
		return;
	}

	// Last position
	if (entity.isDormant()) {
		int r = (int)(Menu::config.espLastPosition[0] * 255);
		int g = (int)(Menu::config.espLastPosition[1] * 255);
		int b = (int)(Menu::config.espLastPosition[2] * 255);
		surface->drawSetColor(r, g, b, 255);
		return;
	}

	// Enemy
	if (entity.isVisible(Terrorist::BoneId::head_0)) {
		surface->drawSetColor(0, 255, 0, 255);
	}
	else {
		surface->drawSetColor(255, 0, 0, 255);
	}
}

void Esp::drawSkeleton(BaseEntity& entity) {
	Surface* surface = (Surface*)m_loader->get(SURFACE);
	Engine* engine = (Engine*)m_loader->get(ENGINE);

	int terroristBoneIds[] = {
		Terrorist::BoneId::head_0, Terrorist::BoneId::pelvis,
		// Left arm
		Terrorist::BoneId::neck_0, Terrorist::BoneId::arm_upper_L,
		Terrorist::BoneId::arm_upper_L, Terrorist::BoneId::arm_lower_L,
		Terrorist::BoneId::arm_lower_L, Terrorist::BoneId::hand_L,
		// Right arm
		Terrorist::BoneId::neck_0, Terrorist::BoneId::arm_upper_R,
		Terrorist::BoneId::arm_upper_R, Terrorist::BoneId::arm_lower_R,
		Terrorist::BoneId::arm_lower_R, Terrorist::BoneId::hand_R,
		// Left leg
		Terrorist::BoneId::pelvis, Terrorist::BoneId::leg_upper_L,
		Terrorist::BoneId::leg_upper_L, Terrorist::BoneId::leg_lower_L,
		Terrorist::BoneId::leg_lower_L, Terrorist::BoneId::ankle_L,
		// Right leg
		Terrorist::BoneId::pelvis, Terrorist::BoneId::leg_upper_R,
		Terrorist::BoneId::leg_upper_R, Terrorist::BoneId::leg_lower_R,
		Terrorist::BoneId::leg_lower_R, Terrorist::BoneId::ankle_R
	};

	int counterTerroristBoneIds[] = {
		CounterTerrorist::BoneId::head_0, CounterTerrorist::BoneId::pelvis,
		// Left arm
		CounterTerrorist::BoneId::neck_0, CounterTerrorist::BoneId::arm_upper_L,
		CounterTerrorist::BoneId::arm_upper_L, CounterTerrorist::BoneId::arm_lower_L,
		CounterTerrorist::BoneId::arm_lower_L, CounterTerrorist::BoneId::hand_L,
		// Right arm
		CounterTerrorist::BoneId::neck_0, CounterTerrorist::BoneId::arm_upper_R,
		CounterTerrorist::BoneId::arm_upper_R, CounterTerrorist::BoneId::arm_lower_R,
		CounterTerrorist::BoneId::arm_lower_R, CounterTerrorist::BoneId::hand_R,
		// Left leg
		CounterTerrorist::BoneId::pelvis, CounterTerrorist::BoneId::leg_upper_L,
		CounterTerrorist::BoneId::leg_upper_L, CounterTerrorist::BoneId::leg_lower_L,
		CounterTerrorist::BoneId::leg_lower_L, CounterTerrorist::BoneId::ankle_L,
		// Right leg
		CounterTerrorist::BoneId::pelvis, CounterTerrorist::BoneId::leg_upper_R,
		CounterTerrorist::BoneId::leg_upper_R, CounterTerrorist::BoneId::leg_lower_R,
		CounterTerrorist::BoneId::leg_lower_R, CounterTerrorist::BoneId::ankle_R
	};


	int nbBones = sizeof(terroristBoneIds) / sizeof(terroristBoneIds[0]);
	int* boneIds = terroristBoneIds;
	if (entity.getTeamNum() == TEAM_COUNTER_TERRORIST) {
		boneIds = counterTerroristBoneIds;
	}

	if (entity.isDormant()) {
		return;
	}
	setEspColor(entity);

	Vector2 a, b;
	Vector3 v;
	bool status;

	for (int i = 0; i < nbBones; i += 2) {
		status = true;

		v = entity.getBonePosition(boneIds[i]);
		if (v.isNull()) {
			continue;
		}
		status &= engine->worldToScreen(v, a);

		v = entity.getBonePosition(boneIds[i+1]);
		if (v.isNull()) {
			continue;
		}
		status &= engine->worldToScreen(v, b);

		if (status) {
			surface->drawLine((int)a.x, (int)a.y, (int)b.x, (int)b.y);
		}
	}
}


void Esp::drawBox(BaseEntity& entity) {
	Surface* surface = (Surface*)m_loader->get(SURFACE);
	Engine* engine = (Engine*)m_loader->get(ENGINE);

	BaseCombatWeapon weapon = BaseCombatWeapon(NULL, m_loader, entity);
	if (!entity.getWeapon(weapon)) {
		return;
	}

	// Foots position
	Vector3 pos3D = entity.getOrigin();
	Vector2 pos2D = Vector2(0.0f, 0.0f);
	if (!engine->worldToScreen(pos3D, pos2D)) {
		return;
	}

	// Head position
	Collideable* collideable = entity.getCollideable();
	if (collideable == NULL) {
		return;
	}
	Vector3 max = collideable->OBBMaxs();
	Vector3 top3D = Vector3(pos3D.x, pos3D.y, pos3D.z + max.z);
	Vector2 top2D = Vector2(0.0f, 0.0f);
	if (!engine->worldToScreen(top3D, top2D)) {
		return;
	}

	// ESP
	float height = pos2D.y - top2D.y;
	float width = height / 3.0f;

	if (entity.isDormant() && !Menu::config.espShowLastPosition) {
		return;
	}
	setEspColor(entity);

	int x0 = (int)(pos2D.x - width);
	int y0 = (int)top2D.y;
	int x1 = (int)(pos2D.x + width);
	int y1 = (int)pos2D.y;
	surface->drawOutlinedRect(x0, y0, x1, y1);
	surface->drawSetColor(0, 0, 0, 255);
	surface->drawOutlinedRect(x0 - 1, y0 - 1, x1 + 1, y1 + 1);
	surface->drawOutlinedRect(x0 + 1, y0 + 1, x1 - 1, y1 - 1);

	// Name
	std::string name;
	entity.getName(name);
	std::string weaponName = (char*) &weapon.getCSWpnData()->m_name;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wname = converter.from_bytes(name) + L" :: " + converter.from_bytes(weaponName);
	surface->drawSetTextPos(x0, y0 - 5);
	surface->drawPrintText(wname.c_str(), wname.size());

	// Health bar
	float health = (float)entity.getHealth();
	int heightBar = (int)(height * (health / 100.0f));
	int widthBar = (int)(3 + height / 100.0f);
	surface->drawSetColor(0, 0, 0, 255);
	surface->drawFilledRect(x0 - widthBar - 1, y0, x0, y1);
	if (health > 60) {
		surface->drawSetColor(0, 255, 0, 255);
	}
	else if (health > 30) {
		surface->drawSetColor(255, 255, 0, 255);
	}
	else {
		surface->drawSetColor(255, 0, 0, 255);
	}

	surface->drawFilledRect(x0 - widthBar, y1 - heightBar, x0 - 1, y1);
}