#pragma once

#include "Vector3.h"

class Collideable {
public:
	virtual void pad0() = 0;
	virtual const Vector3& OBBMins() const = 0;
	virtual const Vector3& OBBMaxs() const = 0;
};