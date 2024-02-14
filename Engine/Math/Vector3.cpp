#include"Vector3.h"
#include<math.h>


// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	return { v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
}


Vector3 Vector3::SetNormalize() {

	float length = sqrtf(x * x + y * y + z * z);
	if (length != 0) {
		Vector3 num = *this /= length;
		x = num.x;
		y = num.y;
		z = num.z;
	}
	return *this;
}

float Vector3::GetLength() {
	return sqrtf(x * x + y * y + z * z);
}




Vector3 Esing(const Vector3& p1, const Vector3& p2, float t)
{
	return{
		p1.x * (1.0f - t) + p2.x * t,
		p1.y * (1.0f - t) + p2.y * t,
		p1.z * (1.0f - t) + p2.z * t,
	};

}

// 内積
float Dot(const Vector3& v1, const Vector3& v2) {
	float Answer = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	return Answer;
}

// 長さ
float Length(const Vector3& v) {
	float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	return length;
}



// 正規化
Vector3 Normalize(const Vector3& v) {
	float length = Length(v);

	if (length != 0) {
		Vector3 Answer = {
			.x = (v.x / length),
			.y = (v.y / length),
			.z = (v.z / length),
		};
		return Answer;
	}
	return v;
}




float Distance(const Vector3& v1, const Vector3& v2) {
	Vector3 v = {
		v2.x - v1.x,
		v2.y - v1.y,
		v2.z - v2.z
	};

	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}


