#pragma once

class Vector3 {
public:
	float x, y, z;
	Vector3();
	Vector3(float x, float y, float z); 
	float lengthSquare();
	float length();
	bool isNull();
	Vector3 operator+(Vector3 v);
	Vector3 operator-(Vector3 v);
	Vector3 operator*(float f);
};