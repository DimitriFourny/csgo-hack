#pragma once

#include "GameInterface.h"
#include "Loader.h"
#include "Vector3.h"
#include "Collideable.h"
#include "EngineTrace.h"
#include "mask.h"
#include "QAngle.h"

class BaseCombatWeapon;

enum Team {
	TEAM_UNK1, 
	TEAM_UNK2,
	TEAM_TERRORIST,
	TEAM_COUNTER_TERRORIST
};

struct PlayerInfo {
	char __pad0[0x8];
	int xuidlow;
	int xuidhigh;
	char name[128];
	int userid;
	char guid[33];
	char __pad1[0x17B];
};

class BaseEntity : public GameInterface {
public:
	BaseEntity(Loader* loader) : BaseEntity(NULL, loader) {};
	BaseEntity(PVOID pObject, Loader* loader);
	bool isEnemy();
	bool isVisible(int bone);
	Vector3 getBonePosition(int bone);
	bool setupBones(Matrix3x4* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime);

	int getTeamNum();
	int getHealth();
	int getAmorValue();
	bool hasElmet();
	Vector3 getOrigin();
	Collideable* getCollideable();
	bool isDormant();
	Vector3 getEyePosition();
	int getIndex();
	void getName(OUT std::string& name);
	bool getWeapon(OUT BaseCombatWeapon& weapon);
	QAngle getPunch();

private:
	static bool m_offsetsInitialized;
	static int m_teamNum;
	static int m_health;
	static int m_armorValue;
	static int m_hashElmet;
	static int m_vecOrigin;
	static int m_collision;
	static int m_isDormant;
	static int m_vecViewOffset;
	static int m_index;
	static int m_activeWeapon;
	static int m_aimPunchAngle;
};