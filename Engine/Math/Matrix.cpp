#include"Matrix.h"
#include<assert.h>
#include<cmath>
#include<numbers>
#include<imgui.h>





void Matrix4x4Debug(const Matrix4x4& m, const char* name) {
	ImGui::Begin(name);
	ImGui::Text("%4.3f ,%4.3f ,%4.3f ,%4.3f ", m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3]);
	ImGui::Text("%4.3f ,%4.3f ,%4.3f ,%4.3f ", m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3]);
	ImGui::Text("%4.3f ,%4.3f ,%4.3f ,%4.3f ", m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3]);
	ImGui::Text("%4.3f ,%4.3f ,%4.3f ,%4.3f ", m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3]);
	ImGui::End();
}

float GetYRotate(const Vector2& v) {
	Vector2 offset = { 0,1 };


	float dot = Dot(offset, v);

	float leng = Length(offset) * Length(v);

	float angle = std::acos(dot / leng);

	if (v.x < 0) {
		angle *= -1;
	}

	

	return angle;

}

float GetXRotate(const Vector2& v) {
	Vector2 offset = { 0,v.x };


	float dot = Dot(offset, v);

	float leng = Length(offset) * Length(v);

	float angle = std::acos(dot / leng);

	if (v.x < 0) {
		angle *= -1;
	}

	

	return angle;

}

Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to) {

	//回転軸
	Vector3 n = Normalize(Cross(from, to));

	if (from == -to) {
		n = Normalize(Vector3{ from.y,-from.x,0 });
		//n = Normalize(Vector3{from.z,0,-from.x });
	}

	//
	float cost = (from * to);

	float sint = +Length(Cross(from, to));

	return {

		(n.x * n.x) * (1 - cost) + cost,      (n.x * n.y) * (1 - cost) + n.z * sint,(n.x * n.z) * (1 - cost) - n.y * sint,0,

		(n.x * n.y) * (1 - cost) - n.z * sint,(n.y * n.y) * (1 - cost) + cost,      (n.y * n.z) * (1 - cost) + n.x * sint,0,

		(n.x * n.z) * (1 - cost) + n.y * sint,(n.y * n.z) * (1 - cost) - n.x * sint,(n.z * n.z) * (1 - cost) + cost,      0,

	0,0,0,1
	};

}


Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, const float angle) {

	return{

		axis.x * axis.x * (1 - std::cos(angle)) + std::cos(angle),
		axis.x * axis.y * (1 - std::cos(angle)) + axis.z * std::sin(angle),
		axis.x * axis.z * (1 - std::cos(angle)) - axis.y * std::sin(angle),
		0,

		axis.x * axis.y * (1 - std::cos(angle)) - axis.z * std::sin(angle),
		axis.y * axis.y * (1 - std::cos(angle)) + std::cos(angle),
		axis.y * axis.z * (1 - std::cos(angle)) + axis.x * std::sin(angle),
		0,

		axis.x * axis.z * (1 - std::cos(angle)) + axis.y * std::sin(angle),
		axis.y * axis.z * (1 - std::cos(angle)) - axis.x * std::sin(angle),
		axis.z * axis.z * (1 - std::cos(angle)) + std::cos(angle),
		0,

		0,
		0,
		0,
		1
	};

}


float CheckR_F_Y(const Vector2& v) { return std::atan2(v.x, v.y); }



Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	return {
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],

	};
}

//透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	return {
		(1.0f / aspectRatio) * (1.0f / std::tan(fovY / 2.0f)),0,0,0,
		0,(1.0f / std::tan(fovY / 2.0f)),0,0,
		0,0,(farClip / (farClip - nearClip)),1,
		0,0,(-nearClip * farClip / (farClip - nearClip)),0
	};
}


//正射投影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	return {
		2.0f / (right - left),0,0,0,
		0,2.0f / (top - bottom),0,0,
		0,0,1.0f / (farClip - nearClip),0,
		(left + right) / (left - right),(top + bottom) / (bottom - top),nearClip / (nearClip - farClip),1,
	};
}

//ビューポート変換
Matrix4x4 MakeViewPortMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	return {
		width / 2.0f,0,0,0,
		0,-(height / 2.0f),0,0,
		0,0,maxDepth - minDepth,0,
		left + (width / 2.0f),top + (height / 2.0f),minDepth,1
	};
}


//座標変換
Vector3 Transform(const Vector3& v, const Matrix4x4& m) {
	Vector3 result = {
		.x{v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + 1.0f * m.m[3][0]},
		.y{v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + 1.0f * m.m[3][1]},
		.z{v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + 1.0f * m.m[3][2] },
	};
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + 1.0f * m.m[3][3];

	assert(w != 0);
	
		return {
			result.x / w,
			result.y / w,
			result.z / w,
		};
	
}


// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 result = { 0 };
	float a = m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] +
		m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] +
		m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] -

		m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] -
		m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] -
		m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] -

		m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] -
		m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] -
		m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] +

		m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] +
		m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] +
		m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +

		m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] +
		m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] +
		m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] -

		m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] -
		m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] -
		m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] -

		m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] -
		m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] -
		m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] +

		m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] +
		m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] +
		m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];
	float lal = sqrtf(a * a);
	result.m[0][0] = (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] +
		m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][1] -
		m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]) /
		lal;
	result.m[0][1] = (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] -
		m.m[0][3] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[2][2] * m.m[3][1] +
		m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]) /
		lal;
	result.m[0][2] = (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] +
		m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][1] -
		m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]) /
		lal;
	result.m[0][3] = (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] -
		m.m[0][3] * m.m[1][1] * m.m[2][2] + m.m[0][3] * m.m[1][2] * m.m[2][1] +
		m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]) /
		lal;

	result.m[1][0] = (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] -
		m.m[1][3] * m.m[2][0] * m.m[3][2] + m.m[1][3] * m.m[2][2] * m.m[3][0] +
		m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]) /
		lal;
	result.m[1][1] = (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] +
		m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][0] -
		m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]) /
		lal;
	result.m[1][2] = (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] -
		m.m[0][3] * m.m[1][0] * m.m[3][2] + m.m[0][3] * m.m[1][2] * m.m[3][0] +
		m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]) /
		lal;
	result.m[1][3] = (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] +
		m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] -
		m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]) /
		lal;

	result.m[2][0] = (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] +
		m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][3] * m.m[2][1] * m.m[3][0] -
		m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]) /
		lal;
	result.m[2][1] = (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] -
		m.m[0][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[2][1] * m.m[3][0] +
		m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]) /
		lal;
	result.m[2][2] = (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] +
		m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][3] * m.m[1][1] * m.m[3][0] -
		m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]) /
		lal;
	result.m[2][3] = (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] -
		m.m[0][3] * m.m[1][0] * m.m[2][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] +
		m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]) /
		lal;

	result.m[3][0] = (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] -
		m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[1][2] * m.m[2][1] * m.m[3][0] +
		m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]) /
		lal;
	result.m[3][1] = (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] +
		m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[2][1] * m.m[3][0] -
		m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]) /
		lal;
	result.m[3][2] = (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] -
		m.m[0][2] * m.m[1][0] * m.m[3][1] + m.m[0][2] * m.m[1][1] * m.m[3][0] +
		m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]) /
		lal;
	result.m[3][3] = (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] +
		m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] -
		m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]) /
		lal;

	return result;
}

// 転置行列
Matrix4x4 Transpose(const Matrix4x4& m) {
	Matrix4x4 NEW = {
		m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0], m.m[0][1], m.m[1][1],
		m.m[2][1], m.m[3][1], m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2],
		m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]

	};
	return NEW;
}
// 単位行列の生成
Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 NEW = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};
	return NEW;
}
// x軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 NEW = {
		1,0,0,0,
		0,std::cos(radian),std::sin(radian),0,
		0,-(std::sin(radian)),std::cos(radian),0,
		0,0,0,1,
	};
	return NEW;
}
// y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 NEW = {
		std::cos(radian), 0, -(std::sin(radian)), 0,
		0, 1, 0, 0,
		std::sin(radian), 0, std::cos(radian),  0,
		0, 0, 0, 1,
	};
	return NEW;
}
// z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 NEW = {
		std::cos(radian),
		std::sin(radian),
		0,
		0,
		-(std::sin(radian)),
		std::cos(radian),
		0,
		0,
		0,
		0,
		1,
		0,
		0,
		0,
		0,
		1,
	};
	return NEW;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	return{
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1
	};
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	return {
		1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			translate.x, translate.y, translate.z, 1,
	};
}

// ３次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	// Scale
	Matrix4x4 S = {
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1 };
	// Rotate
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 R = rotateXMatrix * (rotateYMatrix * rotateZMatrix);
	// translate
	Matrix4x4 T = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		translate.x, translate.y, translate.z, 1,
	};
	Matrix4x4 NEW = S * (R * T);
	return NEW;
}

