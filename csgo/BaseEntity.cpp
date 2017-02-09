#include "BaseEntity.h"
#include "Client.h"
#include "ClientEntList.h"
#include "BaseCombatWeapon.h"
#include <string>

bool BaseEntity::m_offsetsInitialized = false;
int BaseEntity::m_teamNum = -1;
int BaseEntity::m_health = -1;
int BaseEntity::m_armorValue = -1;
int BaseEntity::m_hashElmet = -1;
int BaseEntity::m_vecOrigin = -1;
int BaseEntity::m_collision = -1;
int BaseEntity::m_isDormant = -1;
int BaseEntity::m_vecViewOffset = -1;
int BaseEntity::m_index = -1;
int BaseEntity::m_activeWeapon = -1;
int BaseEntity::m_aimPunchAngle = -1;

BaseEntity::BaseEntity(PVOID pObject, Loader* loader) : GameInterface(pObject, loader) {
	if (!m_offsetsInitialized) {
		Client* client = (Client*)m_loader->get(CLIENT);
		m_teamNum = client->netvarOffset("DT_CSPlayer", "m_iTeamNum");
		m_health = client->netvarOffset("DT_CSPlayer", "m_iHealth");
		m_armorValue = client->netvarOffset("DT_CSPlayer", "m_ArmorValue");
		m_hashElmet = client->netvarOffset("DT_CSPlayer", "m_bHasElmet");
		m_vecOrigin = client->netvarOffset("DT_BasePlayer", "m_vecOrigin");
		m_collision = client->netvarOffset("DT_CSPlayer", "m_Collision");
		m_isDormant = 0xE9;
		m_vecViewOffset = client->netvarOffset("DT_CSPlayer", "m_vecViewOffset[0]");
		m_index = 0x64;
		m_activeWeapon = client->netvarOffset("DT_CSPlayer", "m_hActiveWeapon");
		m_aimPunchAngle = client->netvarOffset("DT_BasePlayer", "m_aimPunchAngle");
		m_offsetsInitialized = true;
	}
}

bool BaseEntity::isEnemy() {
	ClientEntList* clientEntList = (ClientEntList*)m_loader->get(CLIENTENTLIST);
	BaseEntity localPlayer = BaseEntity(m_loader);
	if (!clientEntList->getLocalPlayer(localPlayer)) {
		return false;
	}

	int localTeamNumber = localPlayer.getTeamNum();

	if (localTeamNumber == TEAM_TERRORIST) {
		return (getTeamNum() == TEAM_COUNTER_TERRORIST);
	} else if (localTeamNumber == TEAM_COUNTER_TERRORIST) {
		return (getTeamNum() == TEAM_TERRORIST);
	} 
		
	return false;
}

bool BaseEntity::isVisible(int bone) {
	ClientEntList* clientEntList = (ClientEntList*)m_loader->get(CLIENTENTLIST);
	EngineTrace* engineTrace = (EngineTrace*)m_loader->get(ENGINETRACE);

	BaseEntity localPlayer = BaseEntity(m_loader);
	if (!clientEntList->getLocalPlayer(localPlayer)) {
		return false;
	}

	Ray ray;
	GameTrace tr;
	ray.init(localPlayer.getEyePosition(), getBonePosition(bone)); 

	TraceFilter filter;
	filter.pSkip = localPlayer.self();
	engineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);
	
	return (tr.m_pEnt == self());
}

Vector3 BaseEntity::getBonePosition(int bone) {
	Matrix3x4 boneMatrixes[128];
	
	if (setupBones(boneMatrixes, 128, 0x100, 0)) {
		Matrix3x4 boneMatrix = boneMatrixes[bone];
		return Vector3(boneMatrix.m_matVal[0][3], boneMatrix.m_matVal[1][3], boneMatrix.m_matVal[2][3]);
	} else {
		return Vector3(0, 0, 0);
	}
}

bool BaseEntity::setupBones(Matrix3x4* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime) {
	PVOID pRenderable = (PVOID)((DWORD)self() + sizeof(DWORD));
	VTableHook vtable = VTableHook((DWORD**)pRenderable);

	typedef bool(__thiscall *SetupBonesFn)(PVOID pThis, Matrix3x4* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime);
	SetupBonesFn oSetupBones = (SetupBonesFn) vtable.GetMethodAddress(VMT_SETUPBONES);
	return oSetupBones(pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
}

int BaseEntity::getTeamNum() {
	return *(int*)((DWORD)self() + m_teamNum);
}

int BaseEntity::getHealth() {
	return *(int*)((DWORD)self() + m_health);
}

int BaseEntity::getAmorValue() {
	return *(int*)((DWORD)self() + m_armorValue);
}

bool BaseEntity::hasElmet() {
	return *(bool*)((DWORD)self() + m_hashElmet);
}

Vector3 BaseEntity::getOrigin() {
	return *(Vector3*)((DWORD)self() + m_vecOrigin);
}

Collideable* BaseEntity::getCollideable() {
	return (Collideable*)((DWORD)self() + m_collision);
}

bool BaseEntity::isDormant() {
	return *(bool*)((DWORD)self() + m_isDormant);
}

int BaseEntity::getIndex() {
	return *(int*)((DWORD)self() + m_index);
}

Vector3 BaseEntity::getEyePosition() {
	Vector3 origin = getOrigin();
	Vector3 offset = *(Vector3*)((DWORD)self() + m_vecViewOffset);

	return origin + offset;
}

void BaseEntity::getName(OUT std::string& name) {
	Engine* engine = (Engine*)m_loader->get(ENGINE);
	PlayerInfo playerInfo;
	engine->getPlayerInfo(getIndex(), &playerInfo);
	name = (char*) &playerInfo.name;
}

bool BaseEntity::getWeapon(OUT BaseCombatWeapon& weapon) {
	ClientEntList* clientEntList = (ClientEntList*)m_loader->get(CLIENTENTLIST);
	HANDLE weaponHandle = *(HANDLE*)((DWORD)self() + m_activeWeapon);
	if ((int)weaponHandle == -1) {
		return false;
	}

	BaseEntity entity = BaseEntity(m_loader);
	if (!clientEntList->getClientEntityFromHandle(weaponHandle, entity)) {
		return false;
	}

	weapon.setSelf(entity.self());
	return true;
}

QAngle BaseEntity::getPunch() {
	return *(QAngle*)((DWORD)self() + m_aimPunchAngle);
}