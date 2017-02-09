#include "Math3.h"
#include <math.h>

void Math3::angleToForwardVector(const QAngle& angles, Vector3& forward) {
	float xRad = DEG2RAD(angles.x);
	float yRad = DEG2RAD(angles.y);
	float cosX = (float)cos(xRad);
	float sinX = (float)sin(xRad);
	float cosY = (float)cos(yRad);
	float sinY = (float)sin(yRad);
	forward.x = cosX * cosY;
	forward.y = cosX * sinY;
	forward.z = -sinX;
}

QAngle Math3::angleBtwVectors(Vector3 src, Vector3 dst) {
	QAngle angle;
	Vector3 delta = src - dst;
	angle.x = asinf(delta.z / delta.length()) * M_RADPI;
	angle.y = atanf(delta.y / delta.x) * M_RADPI;
	angle.z = 0.0f;
	if (delta.x >= 0) {
		angle.y += 180;
	}

	return angle;
}
