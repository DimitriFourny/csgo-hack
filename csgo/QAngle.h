#pragma once

class QAngle {
public:
	float x, y, z;
	QAngle();
	QAngle(float x, float y, float z);
	float lengthSquare();
	float length();
	bool isNull();
	void clamp();
	QAngle operator+(QAngle v);
	QAngle operator-(QAngle v);
	QAngle operator*(float f);
};