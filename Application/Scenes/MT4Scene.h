#pragma once
#include"Quaternion.h"

class MT4Scene {
public:
	void Initialize();

	void Update();

	void Draw();

	void Finalize();

private:

	Quaternion rotation = MakeRotateAxisAngleQuaternion(Normalize(Vector3{ 1.0f,0.4f,-0.2f }), 0.45f);

	Vector3 pointY = { 2.1f,-0.9f,1.3f };

	Matrix4x4 rotateMatrix = MakeRotateMatrix(rotation);

	Vector3 rotateByQuaternion = RotateVector(pointY, rotation);

	Vector3 rotateByMatrix = Transform(pointY, rotateMatrix);


};

