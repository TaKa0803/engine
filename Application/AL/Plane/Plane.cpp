#include "Plane.h"
#include<imgui.h>

#include "GlobalVariables/GlobalVariables.h"

Plane::Plane() {
	GameObject::Initialize("plane");
}

//void Plane::CaluclateInit()
//{
//	GlobalVariables* global = GlobalVariables::GetInstance();
//
//	
//	// サイズ比
//	// 3 : 20
//	float stageSize = global->GetFloatvalue("StageWall", "Size");
//	stageSize = stageSize / 20.0f;
//
//	world_.scale_.x = 3.0f * stageSize;
//	world_.scale_.z = 3.0f * stageSize;
//}

void Plane::Initialize() {
	
	world_.scale_ = { 500,500,500 };

	world_.UpdateMatrix();
}

void Plane::DebagWindow() {
	
#ifdef _DEBUG
	float scale = world_.scale_.x;

	ImGui::Begin("stage");
	model_->DebugParameter("stage");
	ImGui::DragFloat3("scale", &world_.scale_.x);
	ImGui::End();

	world_.UpdateMatrix();
#endif // _DEBUG
	
}

void Plane::Draw(const Camera& viewp) {
	world_.UpdateMatrix();
	GameObject::Draw(viewp);
}
