#include "QAngle.h"
#include <math.h>

QAngle::QAngle() {
	x = 0;
	y = 0;
	z = 0;
}

QAngle::QAngle(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

float QAngle::lengthSquare() {
	return (x*x + y*y + z*z);
}

float QAngle::length() {
	return (float)sqrt(lengthSquare());
}

bool QAngle::isNull() {
	return (x < 1 && y < 1 && z < 1);
}

void QAngle::clamp() {
	if (x < -89.0f) {
		x = -89.0f;
	}

	if (x > 89.0f) {
		x = 89.0f;
	}

	while (y < -180.0f) {
		y += 360.0f;
	}
	while (y > 180.0f) {
		y -= 360.0f;
	}

	z = 0.0f;
}

QAngle QAngle::operator+(QAngle v) {
	QAngle r;
	r.x = x + v.x;
	r.y = y + v.y;
	r.z = z + v.z;
	return r;
}

QAngle QAngle::operator-(QAngle v) {
	QAngle r;
	r.x = x - v.x;
	r.y = y - v.y;
	r.z = z - v.z;
	return r;
}

QAngle QAngle::operator*(float f) {
	QAngle r;
	r.x = x * f;
	r.y = y * f;
	r.z = z * f;
	return r;
}