#pragma once

#include "Vector3.h"
#include "QAngle.h"

#define M_PI		3.14159265358979323846f
#define M_RADPI		57.295779513082f
#define RAD2DEG(x)  ( (float)(x) * (float)(180.f / M_PI) )
#define DEG2RAD(x)  ( (float)(x) * (float)(M_PI / 180.f) )


class Math3 {
public:
	static void angleToForwardVector(const QAngle& angles, Vector3& forward);
	static QAngle angleBtwVectors(Vector3 src, Vector3 dst);
};