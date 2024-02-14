#include"IsCollisions.h"

#include <algorithm>


#pragma region 回転量取得関数
//すべてのX回転量取得
float GetAllRotateX(const WorldTransform& world) {
	if (world.parent_ != nullptr) {
		return world.rotate_.x + GetAllRotateX(*world.parent_);
	}
	else {
		return world.rotate_.x;
	}
}

//すべてのY回転量取得
float GetAllRotateY(const WorldTransform& world) {
	if (world.parent_ != nullptr) {
		return world.rotate_.y + GetAllRotateY(*world.parent_);
	}
	else {
		return world.rotate_.y;
	}
}

//すべてのZ回転量取得
float GetAllRotateZ(const WorldTransform& world) {
	if (world.parent_ != nullptr) {
		return world.rotate_.z + GetAllRotateZ(*world.parent_);
	}
	else {
		return world.rotate_.z;
	}
}

//親元も合わせた回転量取得
Vector3 GetAllRotate(const WorldTransform& world) {
	return{
		GetAllRotateX(world),
		GetAllRotateY(world),
		GetAllRotateZ(world)
	};
}
#pragma endregion

#pragma region Scale
float GetAllScaleX(const WorldTransform& world) {
	if (world.parent_ != nullptr) {
		return world.scale_.x * GetAllScaleX(*world.parent_);
	}
	else {
		return world.scale_.x;
	}
}

float GetAllScaleY(const WorldTransform& world) {
	if (world.parent_ != nullptr) {
		return world.scale_.y * GetAllScaleY(*world.parent_);
	}
	else {
		return world.scale_.y;
	}
}

float GetAllScaleZ(const WorldTransform& world) {
	if (world.parent_ != nullptr) {
		return world.scale_.z * GetAllScaleZ(*world.parent_);
	}
	else {
		return world.scale_.z;
	}
}

Vector3 GetAllScale(const WorldTransform& world) {
	return{
		GetAllScaleX(world),
		GetAllScaleY(world),
		GetAllScaleZ(world),
	};
}

#pragma endregion



//AABBと円
bool InCollision(const AABB& a, const Sphere& s, Vector3& v) {
	//最近接点を求める
	Vector3 closestPoint{ std::clamp(s.center.x,a.minV.x,a.maxV.x),
	std::clamp(s.center.y,a.minV.y,a.maxV.y),
	std::clamp(s.center.z,a.minV.z,a.maxV.z)
	};
	Sphere S{
		.center = closestPoint,
		.radius = 0.01f,
	};

	//
	Vector3 V = closestPoint - s.center;

	//最近接点と球の中心との距離を求める
 	float dis = Length(V);
	//距離が半径よりも小さければ衝突
	if (dis <= s.radius) { 
		//最近接点をわたす
		v = closestPoint;
		return true;
	}
	return false;
}


//線分内に点があるか
bool PointInLine(const Vector3 a, const Vector3 LS, const Vector3 LE) {
	//x軸	
	if (LS.x == LE.x) {//垂直
		//設置点aが垂直線の中ではないならfalse
		if (LS.x != a.x) {
			return false;
		}
	}
	else if (LS.x < LE.x) {//LS.xがLE.xより小さい場合の範囲計算
		//xの線の範囲にaがいなければfalse
		if (a.x < LS.x || LE.x < a.x) {
			return false;
		}
	}
	else if (LS.x > LE.x) {//LS.xがLE.xより大きい場合の範囲計算
		//線の範囲内にaがいなければfalse
		if (a.x < LE.x || LS.x < a.x) {
			return false;
		}
	}

	//y軸	
	if (LS.y == LE.y) {//垂直
		//設置点aが垂直線の中ではないならfalse
		if (LS.y != a.y) {
			return false;
		}
	}
	else if (LS.y < LE.y) {//LS.xがLE.xより小さい場合の範囲計算
		//xの線の範囲にaがいなければfalse
		if (a.y < LS.y || LE.y < a.y) {
			return false;
		}
	}
	else if (LS.y > LE.y) {//LS.xがLE.xより大きい場合の範囲計算
		//線の範囲内にaがいなければfalse
		if (a.y < LE.y || LS.y < a.y) {
			return false;
		}
	}

	//z軸	
	if (LS.z == LE.z) {//垂直
		//設置点aが垂直線の中ではないならfalse
		if (LS.z != a.z) {
			return false;
		}
	}
	else if (LS.z < LE.z) {//LS.xがLE.xより小さい場合の範囲計算
		//xの線の範囲にaがいなければfalse
		if (a.z < LS.z || LE.z < a.z) {
			return false;
		}
	}
	else if (LS.z > LE.z) {//LS.xがLE.xより大きい場合の範囲計算
		//線の範囲内にaがいなければfalse
		if (a.z < LE.z || LS.z < a.z) {
			return false;
		}
	}

	return true;
}

bool InCollision(const AABB& AA, const Segment& S) {

	//assert(S.diff == { 0,0,0 });

	//Xplaneと線の点
	Vector3 bx, by, bz;

	bx = { 1,0,0 };
	by = { 0,1,0 };
	bz = { 0,0,1 };


	//tを求める
	float tXmin = (AA.minV.x - S.origin.x) / S.diff.x;
	float tYmin = (AA.minV.y - S.origin.y) / S.diff.y;
	float tZmin = (AA.minV.z - S.origin.z) / S.diff.z;

	float tXmax = (AA.maxV.x - S.origin.x) / S.diff.x;
	float tYmax = (AA.maxV.y - S.origin.y) / S.diff.y;
	float tZmax = (AA.maxV.z - S.origin.z) / S.diff.z;


	float tNearX = std::min(tXmin, tXmax);
	float tNearY = std::min(tYmin, tYmax);
	float tNearZ = std::min(tZmin, tZmax);

	float tFarX = std::max(tXmin, tXmax);
	float tFarY = std::max(tYmin, tYmax);
	float tFarZ = std::max(tZmin, tZmax);


	float tmin = std::max(std::max(tNearX, tNearY), tNearZ);
	float tmax = std::min(std::min(tFarX, tFarY), tFarZ);

	Vector3 a =S.origin+(tmin,S.diff);
	
	Vector3 b = S.origin+(tmax* S.diff);
	

	Vector3 LS, LE;
	LS = S.origin;
	LE = S.origin + S.diff;
	if (tmin <= tmax) {
		if (PointInLine(a, LS, LE) || PointInLine(b, LS, LE)) {
			//衝突
			return true;
		}

	}

	/*
	//最近接点
	Vector3 a = Add(S.origin, Multiply(t, S.diff));
	Sphere Sph = {
		.center{a},
		.radius = 0.01f,
	};
	DrawSphere(Sph, VP, Vport, WHITE);

	//線分内に点があるかチェック
	if (PointInLine(a, AA.min, AA.max)) {
		return true;//早期リターン
	}
	*/
	return false;
}