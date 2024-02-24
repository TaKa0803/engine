#include "GameObject.h"
#include<imgui.h>
GameObject::~GameObject() {
	delete model_;
}

void GameObject::Initialize(const std::string& filepath) {
	model_ = Model::CreateFromOBJ(filepath);
}

void GameObject::Update() {
	world_.UpdateMatrix();
}

void GameObject::Draw(const Camera& camera, Vector3 pointlight, int tex) {
	model_->Draw(world_.matWorld_, camera,pointlight,tex);
}

void GameObject::Debug(const char* name)
{
	if (ImGui::BeginMenu(name)) {

		model_->DebugParameter(name);

		world_.DrawDebug(name);

		ImGui::EndMenu();
	}
}

