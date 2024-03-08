#include "InstancingGameObject/InstancingGameObject.h"
#include<cassert>
#include<imgui.h>

InstancingGameObject::~InstancingGameObject() {
}

void InstancingGameObject::Initialize(const std::string& tag) {
	IMM_ = InstancingModelManager::GetInstance();

	//タグが存在しているかチェック
	if (IMM_->SerchTag(tag)) {
		tag_ = tag;
	}
	else {
		assert(false);
	}



}

void InstancingGameObject::Update() {
	//行列更新
	world_.UpdateMatrix();

}

void InstancingGameObject::Draw()
{
	//タグに対応したモデルにワールド追加
	IMM_->SetData(tag_, world_, color_);
}

void InstancingGameObject::Debug(const char* name)
{
	
		if (ImGui::BeginMenu(name)) {

			IMM_->Debug(tag_, name);

			world_.DrawDebug(name);

			ImGui::EndMenu();
		}
	
}



