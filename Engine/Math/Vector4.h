#pragma once

struct Vector4 {
	float x;
	float y;
	float z;
	float w;

	
	friend Vector4 operator-(const Vector4& v) { return { -v.x,-v.y,-v.z,-v.w }; }
};

Vector4 Esing(const Vector4& st, const Vector4& ed, float t);