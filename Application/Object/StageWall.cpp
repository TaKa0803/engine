#include "StageWall.h"
#include <imgui.h>

void StageWall::SaveNewData()
{
	global_->CreateGroup(cGLOUP);
	//global_->SetValue(cGLOUP, cSIZE, vSize_);
	//global_->SetValue(cGLOUP, cMODELSCALE, vModelScale_);
	//global_->SetValue(cGLOUP, cMODIFIERSCALE, vModifierScale_);
	//global_->SetValue(cGLOUP, cROTATE, world_.rotate_);
	//global_->SetValue(cGLOUP, cNORMALPOSITION, vNormalPosition_);
}

void StageWall::SaveGlobalVariable()
{
	global_->AddItem(cGLOUP, cSIZE, vSize_);
	global_->AddItem(cGLOUP, cMODELSCALE, vModelScale_);
	global_->AddItem(cGLOUP, cMODIFIERSCALE, vModifierScale_);
	global_->AddItem(cGLOUP, cROTATE, world_.rotate_);
	global_->AddItem(cGLOUP, cNORMALPOSITION, vNormalPosition_);
}


void StageWall::LoadGlobalVariable()
{
	vSize_ = global_->GetFloatvalue(cGLOUP, cSIZE);
	vModelScale_ = global_->GetVector3value(cGLOUP, cMODELSCALE);
	vModifierScale_ = global_->GetVector3value(cGLOUP, cMODIFIERSCALE);
	world_.rotate_ = global_->GetVector3value(cGLOUP, cROTATE);
	vNormalPosition_ = global_->GetVector3value(cGLOUP, cNORMALPOSITION);
	// 回転を vNormalPosition と垂直にしてみた
	//world_.rotate_.y = std::atan2f(vNormalPosition_.x, vNormalPosition_.z);
}

void StageWall::CalculateInit()
{
	world_.scale_.x = vModelScale_.x * vModifierScale_.x * (vSize_ / 75.0f);
	world_.scale_.y = vModelScale_.y * vModifierScale_.y;
	world_.scale_.z = vModelScale_.z * vModifierScale_.z;

	// 五角形のどの位置に配置するか
	//vNormalPosition_ = Normalize(vNormalPosition_);
	world_.translate_ = vSize_ * vNormalPosition_;
}

void StageWall::Initialize(size_t num)
{
	std::string tag = "wall";
	InstancingGameObject::Initialize(tag);

	global_ = GlobalVariables::GetInstance();

	// 名前登録
	tag = tag + std::to_string(num);
	//cMODELSCALE = tag + ":" + cMODELSCALE;
	//cMODIFIERSCALE = tag + ":" + cMODIFIERSCALE;
	static int isInit_ = 0;
	if (isInit_ < _COUNT)
	{
		isInit_++;
		cROTATE = tag + ":" + cROTATE;
		cNORMALPOSITION = tag + ":" + cNORMALPOSITION;
	}

	collider_.reset(new OBBCollider);
	collider_->Initialize(tag, world_);



	world_.Initialize();
	LoadGlobalVariable();

	CalculateInit();

	world_.UpdateMatrix();
}

void StageWall::Update()
{
	//行列更新
	world_.UpdateMatrix();
	collider_->Update();
}

void StageWall::Draw()
{
	//タグに対応したモデルにワールド追加
	//IMM_->SetData(tag_, world_, color);

	collider_->Draw();
}

void StageWall::DebagWindow()
{
	// Stage.cpp 側から呼び出す
	//ImGui::Begin("StageWall");

	if (ImGui::Button("LoadConfig"))
	{
		LoadGlobalVariable();

		CalculateInit();
	}

	ImGui::Text("t: %.2f, %.2f %.2f", world_.translate_.x, world_.translate_.y, world_.translate_.z);
	ImGui::Text("r: %.2f, %.2f %.2f", world_.rotate_.x, world_.rotate_.y, world_.rotate_.z);

	//ImGui::End();
	//collider_->Debug("wall");

	world_.UpdateMatrix();
}

void StageWall::OnCollision()
{
}

void StageWall::SetColor(bool hit)
{
	if (hit) {
		color = hitColor;
	}
	else {
		color = baseColor;
	}
}
