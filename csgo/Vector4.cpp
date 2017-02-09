#include "Vector4.h"

Vector4::Vector4() {
	reset();
}

Vector4::Vector4(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

void Vector4::reset() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->w = 0;
}

void Vector4::operator=(Vector3 v) {
	x = v.x;
	y = v.y;
	z = v.z;
	w = 0;
}