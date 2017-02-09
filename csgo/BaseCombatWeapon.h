#pragma once

#include "GameInterface.h"
#include "BaseEntity.h"

enum ItemDefinitionIndex : int {
	WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER = 61,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516
};

class WeaponInfo {
public:
	char pad_0x0000[99];		//0x0000
	char m_name[80];			//0x0063
	char pad_0x07B0[0x74D];		//0x00B3
	float m_flArmorRatio;		//0x0800 
	float unkwn1;				//0x0804 
	float unkwn2;				//0x0808 
	__int32 unkwn3;				//0x080C
	__int32 unkwn4;				//0x0810 
	float m_flPenetration;		//0x0814 
	__int32 m_iDamage;			//0x0818 
	float m_flRange;			//0x081C 
	float m_flRangeModifier;	//0x0820 
};

class BaseCombatWeapon : public GameInterface {
public:
	BaseCombatWeapon(PVOID pObject, Loader* loader, BaseEntity& player);
	~BaseCombatWeapon();
	int getWeaponId();
	bool isGun();
	bool isSniperRifle();
	bool isEmpty();
	bool isReloading();
	int zoomLevel();
	WeaponInfo* getCSWpnData();

private:
	static int m_offsetsInitialized;
	static int m_id;
	static int m_clip1;
	static int m_isReloading;
	static int m_zoomLevel;
};