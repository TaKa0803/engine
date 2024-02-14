#include "BoxColider.h"
#include"TextureManager/TextureManager.h"
#include"imgui.h"

//クロス積
static Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	return { v1.y * v2.z - v1.z * v2.y,v1.z * v2.x - v1.x * v2.z,v1.x * v2.y - v1.y * v2.x };
}

/*
//AABB同士
bool InCollision(const AABB& a, const AABB& b) {

	if ((a.minV.x <= b.maxV.x && a.maxV.x >= b.minV.x) &&//x軸
		(a.minV.y <= b.maxV.y && a.maxV.y >= b.minV.y) &&
		(a.minV.z <= b.maxV.z && a.maxV.z >= b.minV.z)
		) {
		return true;
	}
	return false;
}
*/

void CheckswitchAABB(AABB& a) {
	Vector3 max{}, min{};
	min.x = (std::min)(a.minV.x, a.maxV.x);
	min.y = (std::min)(a.minV.y, a.maxV.y);
	min.z = (std::min)(a.minV.z, a.maxV.z);

	max.x = (std::max)(a.minV.x, a.maxV.x);
	max.y = (std::max)(a.minV.y, a.maxV.y);
	max.z = (std::max)(a.minV.z, a.maxV.z);
	a.maxV = max;
	a.minV = min;

}

#pragma region コライダー
Collider::~Collider() {
	delete model_;
}

void Collider::Initialize(const WorldTransform* world) {


	world_.parent_=(world);

	model_ = Model::CreateFromOBJ("ALPlayer");

	model_->SetFillMode(FillMode::kWireFrame);

	texture_ = TextureManager::LoadTex("resources/Plane.png");

}

void Collider::Update() {
	world_.UpdateMatrix();
}

void Collider::Draw(const Matrix4x4& VP) {
	model_->Draw(world_.matWorld_, VP, texture_);
}
#pragma endregion



#pragma region BoxCollider

BoxColider::~BoxColider() {
	//Collider::~Collider();
}

void BoxColider::Initialize(const WorldTransform* world) {
	Collider::Initialize(world);

	type = ColliderType::Box;


}

void BoxColider::Update() {
	Collider::Update();
	aabb = {
		.minV{world_.GetMatWorldTranslate().x - world_.scale_.x,world_.GetMatWorldTranslate().y - world_.scale_.y,world_.GetMatWorldTranslate().z - world_.scale_.z},
		.maxV{world_.GetMatWorldTranslate().x + world_.scale_.x,world_.GetMatWorldTranslate().y + world_.scale_.y,world_.GetMatWorldTranslate().z + world_.scale_.z},
	};

	CheckswitchAABB(aabb);
}

void BoxColider::Draw(const Matrix4x4& VP) {
	Collider::Draw(VP);
	

}

void BoxColider::DebugImGui(const char* name) {
#ifdef _DEBUG
	ImGui::Begin(name);
	ImGui::DragFloat3("pos", &world_.translate_.x);
	ImGui::DragFloat3("scale", &world_.scale_.x);
	ImGui::End();
#endif // _DEBUG

}



void BoxColider::Collision(const Collider& collider, Vector3& pushBackVelo) {
	Collider::Update();

	aabb = {
		.minV{world_.translate_.x - world_.scale_.x,world_.translate_.y - world_.scale_.y,world_.translate_.z - world_.scale_.z},
		.maxV{world_.translate_.x + world_.scale_.x,world_.translate_.y + world_.scale_.y,world_.translate_.z + world_.scale_.z},
	};

	CheckswitchAABB(aabb);


	if (type == ColliderType::Box) {
		if (collider.type == ColliderType::Box) {
			//当たっていた場合
			//if (InCollision(aabb, collider.aabb)) {

				Vector3 AB = collider.aabb.maxV-collider.aabb.minV;

				Vector3 AC = collider.world_.GetMatWorldTranslate() - collider.aabb.minV;

				Vector3 normal = Normalize(Cross(AB,AC));

				//Cross積1
				//Vector3 velo1 = collider.aabb.maxV - collider.aabb.minV;
				//2
				Vector3 velo2 = collider.world_.GetMatWorldTranslate() - collider.aabb.maxV;


				Vector3 cross = Cross(AB, velo2);
				
				//内積
				//プラスなら上方向に押し出し
				if (Dot(cross, normal) >= 0.0f) {
					pushBackVelo.x;
				}


				pushBackVelo = {
					//xの押し戻し量
					collider.aabb.maxV.x,
				};
			//}

		}

	}
}

#pragma endregion







