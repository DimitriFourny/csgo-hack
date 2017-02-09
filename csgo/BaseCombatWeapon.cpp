#include "BaseCombatWeapon.h"
#include "Client.h"
#include "Loader.h"

int BaseCombatWeapon::m_offsetsInitialized = false;
int BaseCombatWeapon::m_id = -1;
int BaseCombatWeapon::m_clip1 = -1;
int BaseCombatWeapon::m_isReloading = -1;
int BaseCombatWeapon::m_zoomLevel = -1;

BaseCombatWeapon::BaseCombatWeapon(PVOID pObject, Loader* loader, BaseEntity& player) : GameInterface(pObject, loader) {
	if (!m_offsetsInitialized) {
		Client* client = (Client*)m_loader->get(CLIENT);
		m_id = client->netvarOffset("DT_WeaponCSBase", "m_fAccuracyPenalty") + 0x30;
		m_clip1 = client->netvarOffset("DT_BaseCombatWeapon", "m_iClip1");
		m_isReloading = 0x3235;
		m_zoomLevel = client->netvarOffset("DT_WeaponCSBaseGun", "m_zoomLevel");
		m_offsetsInitialized = true;
	}
}

BaseCombatWeapon::~BaseCombatWeapon() {

}

int BaseCombatWeapon::getWeaponId() {
	return *(int*)((DWORD)self() + m_id);
}

bool BaseCombatWeapon::isGun() {
	int id = getWeaponId();

	switch (id) {
		case WEAPON_DEAGLE:
		case WEAPON_ELITE:
		case WEAPON_FIVESEVEN:
		case WEAPON_GLOCK:
		case WEAPON_AK47:
		case WEAPON_AUG:
		case WEAPON_AWP:
		case WEAPON_FAMAS:
		case WEAPON_G3SG1:
		case WEAPON_GALILAR:
		case WEAPON_M249:
		case WEAPON_M4A1:
		case WEAPON_MAC10:
		case WEAPON_P90:
		case WEAPON_UMP45:
		case WEAPON_XM1014:
		case WEAPON_BIZON:
		case WEAPON_MAG7:
		case WEAPON_NEGEV:
		case WEAPON_SAWEDOFF:
		case WEAPON_TEC9:
			return true;
		case WEAPON_TASER:
			return false;
		case WEAPON_HKP2000:
		case WEAPON_MP7:
		case WEAPON_MP9:
		case WEAPON_NOVA:
		case WEAPON_P250:
		case WEAPON_SCAR20:
		case WEAPON_SG556:
		case WEAPON_SSG08:
			return true;
		case WEAPON_KNIFE:
		case WEAPON_FLASHBANG:
		case WEAPON_HEGRENADE:
		case WEAPON_SMOKEGRENADE:
		case WEAPON_MOLOTOV:
		case WEAPON_DECOY:
		case WEAPON_INCGRENADE:
		case WEAPON_C4:
		case WEAPON_KNIFE_T:
			return false;
		case WEAPON_M4A1_SILENCER:
		case WEAPON_USP_SILENCER:
		case WEAPON_CZ75A:
		case WEAPON_REVOLVER:
			return true;
		default:
			return false;
	}
}

bool BaseCombatWeapon::isSniperRifle() {
	int id = getWeaponId();

	switch (id) {
	case WEAPON_AWP:
	case WEAPON_G3SG1:
	case WEAPON_SSG08:
	case WEAPON_SCAR20:
		return true;
	default:
		return false;
	}
}

bool BaseCombatWeapon::isEmpty() {
	int clip1 = *(int*)((DWORD)self() + m_clip1);
	return clip1 == 0;
}

bool BaseCombatWeapon::isReloading() {
	return *(bool*)((DWORD)self() + m_isReloading);
}

int BaseCombatWeapon::zoomLevel() {
	return *(int*)((DWORD)self() + m_zoomLevel);
}

WeaponInfo* BaseCombatWeapon::getCSWpnData() {
	typedef WeaponInfo*(__thiscall *GetCSWpnDataFn)(PVOID pThis);
	GetCSWpnDataFn oGetCSWpnData = (GetCSWpnDataFn)m_vtable->GetMethodAddress(VMT_GETCSWPNDATA);
	return oGetCSWpnData(self());
}