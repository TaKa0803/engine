#pragma once
#include"Math/Vector3.h"
#include"Math/Matrix.h"

class Quaternion {

public:

	float x=0;

	float y=0;

	float z=0;

	float w=0;

	inline Vector3 XYZ()const { return { x,y,z }; }
	
#pragma region オーバーロード

	friend Quaternion operator-(const Quaternion& v) { return { -v.x,-v.y,-v.z,-v.w }; }
	friend Quaternion operator+(const Quaternion& v) { return v; }


	friend Quaternion operator*(const float scalar, const Quaternion& v) { return { v.x * scalar, v.y * scalar, v.z * scalar,v.w * scalar }; }
	friend Quaternion operator*(const Quaternion& q, const float f) { return { q.x * f,q.y * f,q.z * f,q.w * f }; }
	friend Quaternion operator*(const Quaternion& q, const Quaternion& r) {

		Vector3 qv = Vector3{ q.x, q.y, q.z };

		Vector3 rv = Vector3{ r.x, r.y, r.z };;

		Vector3 qrv = Cross(qv, rv) + r.w * qv + q.w * rv;

		Quaternion ans;
		ans.x = qrv.x;
		ans.y = qrv.y;
		ans.z = qrv.z;
		ans.w = q.w * r.w - qv * rv;

		return ans;
	}
	friend Quaternion operator+(const Quaternion& q1, const Quaternion& q2) {
		return { .x{q1.x + q2.x},.y{q1.y + q2.y},.z{q1.z + q2.z},.w{q1.w + q2.w} };
	}

	
	Quaternion& operator*=(float s) { x *= s; y *= s; z *= s; w *= s; return *this; }
#pragma endregion


};

float Dot(const Quaternion& q1, const Quaternion& q2);

Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, const float t);

//任意軸回転を表すQuaternionの生成
Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);

//ベクトルをQuaternionで回転させた結果のベクトルを求める
Vector3 RotateVector(const Vector3& vec, const Quaternion& qua);

Matrix4x4 MakeRotateMatrix(const Quaternion& qua);

Quaternion MakeIdentityQuaternion();

//共役
Quaternion Conjugate(const Quaternion& que);

float Norm(const Quaternion& que);

Quaternion Normalize(const Quaternion& que);

Quaternion Inverse(const Quaternion& que);
