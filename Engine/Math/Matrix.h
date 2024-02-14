#pragma once
#include"Vector3.h"
#include"Vector2.h"

struct Matrix4x4 {
	float m[4][4];

	Matrix4x4& operator+=(const Matrix4x4& m1) {
		*this = {
		(m[0][0] + m1.m[0][0]), (m[0][1] + m1.m[0][1]), (m[0][2] + m1.m[0][2]), (m[0][3] + m1.m[0][3]),
		(m[1][0] + m1.m[1][0]), (m[1][1] + m1.m[1][1]), (m[1][2] + m1.m[1][2]), (m[1][3] + m1.m[1][3]),
		(m[2][0] + m1.m[2][0]), (m[2][1] + m1.m[2][1]), (m[2][2] + m1.m[2][2]), (m[2][3] + m1.m[2][3]),
		(m[3][0] + m1.m[3][0]), (m[3][1] + m1.m[3][1]), (m[3][2] + m1.m[3][2]), (m[3][3] + m1.m[3][3]),
		};
		return *this;
	}

	Matrix4x4& operator-=(const Matrix4x4& m1) {
		*this = {
		(m[0][0] - m1.m[0][0]), (m[0][1] - m1.m[0][1]), (m[0][2] - m1.m[0][2]), (m[0][3] - m1.m[0][3]),
		(m[1][0] - m1.m[1][0]), (m[1][1] - m1.m[1][1]), (m[1][2] - m1.m[1][2]), (m[1][3] - m1.m[1][3]),
		(m[2][0] - m1.m[2][0]), (m[2][1] - m1.m[2][1]), (m[2][2] - m1.m[2][2]), (m[2][3] - m1.m[2][3]),
		(m[3][0] - m1.m[3][0]), (m[3][1] - m1.m[3][1]), (m[3][2] - m1.m[3][2]), (m[3][3] - m1.m[3][3]),
		};
		return *this;
	}

	Matrix4x4& operator*=(const Matrix4x4& m1) {
		*this = {
			m[0][0] * m1.m[0][0] + m[0][1] * m1.m[1][0] + m[0][2] * m1.m[2][0] + m[0][3] * m1.m[3][0],
			m[0][0] * m1.m[0][1] + m[0][1] * m1.m[1][1] + m[0][2] * m1.m[2][1] + m[0][3] * m1.m[3][1],
			m[0][0] * m1.m[0][2] + m[0][1] * m1.m[1][2] + m[0][2] * m1.m[2][2] + m[0][3] * m1.m[3][2],
			m[0][0] * m1.m[0][3] + m[0][1] * m1.m[1][3] + m[0][2] * m1.m[2][3] + m[0][3] * m1.m[3][3],

			m[1][0] * m1.m[0][0] + m[1][1] * m1.m[1][0] + m[1][2] * m1.m[2][0] + m[1][3] * m1.m[3][0],
			m[1][0] * m1.m[0][1] + m[1][1] * m1.m[1][1] + m[1][2] * m1.m[2][1] + m[1][3] * m1.m[3][1],
			m[1][0] * m1.m[0][2] + m[1][1] * m1.m[1][2] + m[1][2] * m1.m[2][2] + m[1][3] * m1.m[3][2],
			m[1][0] * m1.m[0][3] + m[1][1] * m1.m[1][3] + m[1][2] * m1.m[2][3] + m[1][3] * m1.m[3][3],

			m[2][0] * m1.m[0][0] + m[2][1] * m1.m[1][0] + m[2][2] * m1.m[2][0] + m[2][3] * m1.m[3][0],
			m[2][0] * m1.m[0][1] + m[2][1] * m1.m[1][1] + m[2][2] * m1.m[2][1] + m[2][3] * m1.m[3][1],
			m[2][0] * m1.m[0][2] + m[2][1] * m1.m[1][2] + m[2][2] * m1.m[2][2] + m[2][3] * m1.m[3][2],
			m[2][0] * m1.m[0][3] + m[2][1] * m1.m[1][3] + m[2][2] * m1.m[2][3] + m[2][3] * m1.m[3][3],

			m[3][0] * m1.m[0][0] + m[3][1] * m1.m[1][0] + m[3][2] * m1.m[2][0] + m[3][3] * m1.m[3][0],
			m[3][0] * m1.m[0][1] + m[3][1] * m1.m[1][1] + m[3][2] * m1.m[2][1] + m[3][3] * m1.m[3][1],
			m[3][0] * m1.m[0][2] + m[3][1] * m1.m[1][2] + m[3][2] * m1.m[2][2] + m[3][3] * m1.m[3][2],
			m[3][0] * m1.m[0][3] + m[3][1] * m1.m[1][3] + m[3][2] * m1.m[2][3] + m[3][3] * m1.m[3][3],
		};
		return *this;
	}

	friend Matrix4x4 operator+(const Matrix4x4& m, const Matrix4x4& m1) { return {
		(m.m[0][0] + m1.m[0][0]), (m.m[0][1] + m1.m[0][1]), (m.m[0][2] + m1.m[0][2]), (m.m[0][3] + m1.m[0][3]),
		(m.m[1][0] + m1.m[1][0]), (m.m[1][1] + m1.m[1][1]), (m.m[1][2] + m1.m[1][2]), (m.m[1][3] + m1.m[1][3]),
		(m.m[2][0] + m1.m[2][0]), (m.m[2][1] + m1.m[2][1]), (m.m[2][2] + m1.m[2][2]), (m.m[2][3] + m1.m[2][3]),
		(m.m[3][0] + m1.m[3][0]), (m.m[3][1] + m1.m[3][1]), (m.m[3][2] + m1.m[3][2]), (m.m[3][3] + m1.m[3][3]),
	}; }
	friend Matrix4x4 operator-(const Matrix4x4& m, const Matrix4x4& m1) { return {
		(m.m[0][0] - m1.m[0][0]), (m.m[0][1] - m1.m[0][1]), (m.m[0][2] - m1.m[0][2]), (m.m[0][3] - m1.m[0][3]),
		(m.m[1][0] - m1.m[1][0]), (m.m[1][1] - m1.m[1][1]), (m.m[1][2] - m1.m[1][2]), (m.m[1][3] - m1.m[1][3]),
		(m.m[2][0] - m1.m[2][0]), (m.m[2][1] - m1.m[2][1]), (m.m[2][2] - m1.m[2][2]), (m.m[2][3] - m1.m[2][3]),
		(m.m[3][0] - m1.m[3][0]), (m.m[3][1] - m1.m[3][1]), (m.m[3][2] - m1.m[3][2]), (m.m[3][3] - m1.m[3][3]),
	};; }
	friend Matrix4x4 operator*(const Matrix4x4& m, const Matrix4x4& m1) { return {
			m.m[0][0] * m1.m[0][0] + m.m[0][1] * m1.m[1][0] + m.m[0][2] * m1.m[2][0] + m.m[0][3] * m1.m[3][0],
			m.m[0][0] * m1.m[0][1] + m.m[0][1] * m1.m[1][1] + m.m[0][2] * m1.m[2][1] + m.m[0][3] * m1.m[3][1],
			m.m[0][0] * m1.m[0][2] + m.m[0][1] * m1.m[1][2] + m.m[0][2] * m1.m[2][2] + m.m[0][3] * m1.m[3][2],
			m.m[0][0] * m1.m[0][3] + m.m[0][1] * m1.m[1][3] + m.m[0][2] * m1.m[2][3] + m.m[0][3] * m1.m[3][3],

			m.m[1][0] * m1.m[0][0] + m.m[1][1] * m1.m[1][0] + m.m[1][2] * m1.m[2][0] + m.m[1][3] * m1.m[3][0],
			m.m[1][0] * m1.m[0][1] + m.m[1][1] * m1.m[1][1] + m.m[1][2] * m1.m[2][1] + m.m[1][3] * m1.m[3][1],
			m.m[1][0] * m1.m[0][2] + m.m[1][1] * m1.m[1][2] + m.m[1][2] * m1.m[2][2] + m.m[1][3] * m1.m[3][2],
			m.m[1][0] * m1.m[0][3] + m.m[1][1] * m1.m[1][3] + m.m[1][2] * m1.m[2][3] + m.m[1][3] * m1.m[3][3],

			m.m[2][0] * m1.m[0][0] + m.m[2][1] * m1.m[1][0] + m.m[2][2] * m1.m[2][0] + m.m[2][3] * m1.m[3][0],
			m.m[2][0] * m1.m[0][1] + m.m[2][1] * m1.m[1][1] + m.m[2][2] * m1.m[2][1] + m.m[2][3] * m1.m[3][1],
			m.m[2][0] * m1.m[0][2] + m.m[2][1] * m1.m[1][2] + m.m[2][2] * m1.m[2][2] + m.m[2][3] * m1.m[3][2],
			m.m[2][0] * m1.m[0][3] + m.m[2][1] * m1.m[1][3] + m.m[2][2] * m1.m[2][3] + m.m[2][3] * m1.m[3][3],

			m.m[3][0] * m1.m[0][0] + m.m[3][1] * m1.m[1][0] + m.m[3][2] * m1.m[2][0] + m.m[3][3] * m1.m[3][0],
			m.m[3][0] * m1.m[0][1] + m.m[3][1] * m1.m[1][1] + m.m[3][2] * m1.m[2][1] + m.m[3][3] * m1.m[3][1],
			m.m[3][0] * m1.m[0][2] + m.m[3][1] * m1.m[1][2] + m.m[3][2] * m1.m[2][2] + m.m[3][3] * m1.m[3][2],
			m.m[3][0] * m1.m[0][3] + m.m[3][1] * m1.m[1][3] + m.m[3][2] * m1.m[2][3] + m.m[3][3] * m1.m[3][3],
	};
	}

};

void Matrix4x4Debug(const Matrix4x4& m, const char* name);


float GetYRotate(const Vector2& v);

float GetXRotate(const Vector2& v);

Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to);


Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);


Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, const float angle);


//透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

//正射投影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

//ビューポート変換
Matrix4x4 MakeViewPortMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);


//座標変換
Vector3 Transform(const Vector3& v, const Matrix4x4& m);



// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);
// 転置行列
Matrix4x4 Transpose(const Matrix4x4& m);
// 単位行列の生成
Matrix4x4 MakeIdentity4x4();

// x軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
// y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);
// z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);


Matrix4x4 MakeScaleMatrix(const Vector3& scale);

Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

//Matrix4x4 MakeAffineMatrix(Matrix4x4 scale, Matrix4x4 rotate, Matrix4x4 translate);

