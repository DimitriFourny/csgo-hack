#pragma once

#include "Vector3.h"

class Vector4 : public Vector3 {
public:
	float w;
	Vector4();
	Vector4(float x, float y, float z, float w);
	void reset();
	void operator=(Vector3 v);
};