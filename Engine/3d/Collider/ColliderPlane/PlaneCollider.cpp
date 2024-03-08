#include"PlaneCollider.h"
#include<imgui.h>






struct Plane
{
	Vector3 normal;
	float distance;
};


////円と面
//bool InCollision(const Sphere& sphere, const Plane& plane) {
//	Vector3 v1 = sphere.center;
//	Vector3 v2 = plane.distance * plane.normal;
//	if (Length(v1 - v2) <= sphere.radius) {
//		return true;
//	}
//	return false;
//}

void PlaneCollider::Initialize(const std::string& tag, const WorldTransform& parent)
{
	InstancingGameObject::Initialize("z+Plane");

	world_.parent_=(&parent);

	colliderTag_ = tag;

	IMM_->SetEnableTexture(tag_, false);
	IMM_->SetFillMode(tag_, FillMode::kWireFrame);
}

void PlaneCollider::Update()
{

	world_.UpdateMatrix();


	normal = TransformNormal(offset, world_.matWorld_);


}

void PlaneCollider::Draw()
{
#ifdef _DEBUG
	if (isDraw_) {
		InstancingModelManager::GetInstance()->SetData(tag_, world_,color_);
	}
#endif // _DEBUG
}

void PlaneCollider::Debug(const char* name)
{
#ifdef _DEBUG
	if (!ImGui::BeginMenu(name)) {



		ImGui::EndMenu();
	}
#endif // _DEBUG


}
