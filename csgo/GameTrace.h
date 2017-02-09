#pragma once

#include "Vector3.h"

typedef unsigned char byte;

struct Plane {
	Vector3	normal;
	float	dist;
	byte	type;
	byte	signbits;
	byte	pad[2];
};

class BaseTrace {
public:
	Vector3 startpos;
	Vector3 endpos;
	Plane plane;
	float fraction;
	int contents;
	unsigned short dispFlags;
	bool allsolid;
	bool startsolid;
};

struct Surface_t {
	const char*		name;
	short			surfaceProps;
	unsigned short	flags;
};

class BaseEntity;
class GameTrace : public BaseTrace {
public:
	float fractionleftsolid;
	Surface_t surface;
	int hitgroup;
	short physicsbone;
	unsigned short worldSurfaceIndex;
	BaseEntity* m_pEnt;
	int hitbox;
};