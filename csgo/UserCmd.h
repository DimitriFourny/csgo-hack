#pragma once

#include "QAngle.h"
#include "Vector3.h"

typedef unsigned char byte;

class UserCmd {
	public:
		virtual ~UserCmd() { };
		int		command_number;
		int		tick_count;
		QAngle	viewangles;
		Vector3	aimdirection;
		float	forwardmove;
		float	sidemove;
		float	upmove;
		int		buttons;
		byte    impulse;
		int		weaponselect;
		int		weaponsubtype;
		int		random_seed;
		short	mousedx;
		short	mousedy;
		bool	hasbeenpredicted;
		char	pad_0x4C[0x18];
};