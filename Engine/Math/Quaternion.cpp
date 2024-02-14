#include "Quaternion.h"
#include<cmath>



Quaternion MakeIdentityQuaternion() {

	return {
		0,0,0,1
	};

}

Quaternion Conjugate(const Quaternion& que) {
	Vector3 v3 = { que.x,que.y,que.z };



	Quaternion ans = {
		-v3.x,
		-v3.y,
		-v3.z,
		que.w
	};
	return ans;
}

float Norm(const Quaternion& que) {
	return sqrtf(que.w * que.w + que.x * que.x + que.y * que.y + que.z * que.z);
}

Quaternion Normalize(const Quaternion& que) {
	float norm = Norm(que);

	return {
		que.x / norm,
		que.y / norm,
		que.z / norm,
		que.w / norm,
	};

}

Quaternion Inverse(const Quaternion& que) {
	Quaternion q = Conjugate(que);
	float norm = Norm(que);

	return q * (1.0f / (norm * norm));

}


float Dot(const Quaternion& q0, const Quaternion& q1)
{
	return { q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w };
}

Quaternion Slerp(const Quaternion& q, const Quaternion& qq, const float t)
{
	Quaternion q0 = q;

	Quaternion q1 = qq;

	float dot = Dot(q0, q1);
	
	if (dot < 0) {
		q0 *= -1;
		dot*=-1;
	}

	float EPSILON = 0.0005f;

	if (dot >= 1.0f - EPSILON) {
		return (1.0f - t) * q0 + t * q1;
	}

	//なす角を求める
	float theta = std::acos(dot);


	q0 *= (std::sin((1 - t) * theta) / sin(theta));

	q1 *= (std::sin(t * theta) / std::sin(theta));

	return q0+q1;
}

Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {

	Quaternion ans;

	
	float theta = angle / 2.0f;

	ans = {
		axis.x * std::sin(theta),
		axis.y * std::sin(theta),
		axis.z * std::sin(theta),
		std::cos(theta),
	};

	

	return ans;
}

Vector3 RotateVector(const Vector3& vec, const Quaternion& qua) {

	Quaternion qDash = Conjugate(qua);

	Quaternion r = { vec.x,vec.y,vec.z,0 };

	Quaternion ans=qua * r * qDash;

	return { ans.x,ans.y,ans.z };

}

Matrix4x4 MakeRotateMatrix(const Quaternion& q) {
	return {

		q.w*q.w+q.x*q.x-q.y*q.y-q.z*q.z,2*(q.x*q.y+q.w*q.z),2*(q.x*q.z-q.w*q.y),0,

		2*(q.x*q.y-q.w*q.z),q.w* q.w - q.x * q.x + q.y * q.y - q.z * q.z,2*(q.y*q.z+q.w*q.x),0,

		2*(q.x*q.z+q.w*q.y),2*(q.y*q.z-q.w*q.x),q.w* q.w - q.x * q.x - q.y * q.y + q.z * q.z,0,
		0,0,0,1

	};
}