#pragma once
#include"Math/Vector4.h"
#include"Math/Vector3.h"
#include"Math/Matrix.h"
#include"WorldTransform/WorldTransform.h"

struct Particle {
	WorldTransform world_;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
	float currentTime;
};

struct Particle4GPU {
	Matrix4x4 WVP;
	Matrix4x4 World;
	Vector4 color;

};


struct MinMaxDataV3 {
	Vector3 minData;
	Vector3 maxData;
};

struct MinMaxDataV4 {
	Vector4 minData;
	Vector4 maxData;
};

Particle MakeNewParticle(const Vector3& spawnPos, const Vector3& emiterSize, const MinMaxDataV3&velo, const MinMaxDataV4& color, const Vector2& lifeTimeminXmaxY);