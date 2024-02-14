#include"Vector2.h"
#include<math.h>


// 長さ
float Length(const Vector2& v) {
	float length = sqrtf(v.x * v.x + v.y * v.y);
	return length;
}

Vector2 Normalize(const Vector2& v) {
	float length = Length(v);

	if (length != 0) {
		Vector2 Answer = {
			.x = (v.x / length),
			.y = (v.y / length),
		};
		return Answer;
	}
	return{ 0,0 };
}

float Dot(const Vector2& v1, const Vector2& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

Vector2 Vector2::Normalize() {
	float leng = sqrtf(x * x + y * y);

	if (leng != 0) {
		x /= leng;
		y /= leng;
	}

	return *this;
}
