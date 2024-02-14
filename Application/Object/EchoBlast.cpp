#include "EchoBlast.h"

#include "SphereCollider/SphereCollider.h"
#include "ImGuiManager/ImGuiManager.h"

void EchoBlast::Initialize(const Infomation& info)
{
	world_.Initialize();
	colliderWorld_.Initialize();


	collider_.reset(new OBBCollider);
	collider_->Initialize("echo", colliderWorld_);

	SetInfomation(info);

	switch (data_.mode_)
	{
	case ATTACKMODE::aSPOT:
		InstancingGameObject::Initialize("PAmmo");
		data_.power_ = 2.0f;
		aliveFrame_ = 40;
		break;
	case ATTACKMODE::aMIRROR:
		InstancingGameObject::Initialize("SRS");
		collider_->SetScale({ 5.0f,1.0f,1.0f });
		colliderWorld_.translate_.z = 2.0f;
		data_.power_ = 1.0f;
		aliveFrame_ = 20;
		break;
	}

	//switch (info.mode_)
	//{
	//case ATTACKMODE::aSPOT:
	//	InstancingGameObject::Initialize("PAmmo");
	//	data_.power_ = 2.0f;
	//	aliveFrame_ = 40;
	//	break;
	//case ATTACKMODE::aMIRROR:
	//	InstancingGameObject::Initialize("SRS");
	//	collider_->SetScale({ 5.0f,1.0f,1.0f });
	//	data_.power_ = 1.0f;
	//	aliveFrame_ = 60;
	//	break;
	//}

	//direct3_ = { data_.direction_.x,0.0f,data_.direction_.y };
	//world_.translate_ = data_.popPosition_ + direct3_ * 0.3f;
	//world_.rotate_.y = GetYRotate(info.direction_);
	//colliderWorld_.parent_ = &world_;

	////colliderWorld_.translate_.y = 1.0f;

	//isActive_ = true;
	//collider_->SetColor({ 0.0f,0.0f,0.0f,1.0f });
}

void EchoBlast::Update()
{
	aliveFrame_--;
	if (aliveFrame_ <= 0)
	{
		isActive_ = false;
	}
	switch (data_.mode_)
	{
	case ATTACKMODE::aSPOT:
		world_.translate_ += direct3_ * (data_.power_ * 0.1f);
		break;
	case ATTACKMODE::aMIRROR:
		break;
	default:
		break;
	}
	//行列更新
	world_.UpdateMatrix();
	colliderWorld_.UpdateMatrix();
	collider_->Update();
	//sCollider_->Update();
	//タグに対応したモデルにワールド追加
	IMM_->SetData(tag_, colliderWorld_);
}

void EchoBlast::Draw()
{
	if (isActive_)
	{
		collider_->Draw();
		//sCollider_->Draw();
	}
}

void EchoBlast::DebugWindow()
{
	collider_->Debug("echo");

	ImGui::Begin(tag_.c_str());

	ImGui::Text("tra : %.2f,%.2f", world_.translate_.x, world_.translate_.z);
	ImGui::Text("dir : %.2f,%.2f", direct3_.x, direct3_.z);
	ImGui::Separator();

	ImGui::End();
}

void EchoBlast::OnCollision()
{
}

void EchoBlast::SetInfomation(const Infomation& info)
{
	data_ = info;
	direct3_ = { data_.direction_.x,0.0f,data_.direction_.y };
	world_.translate_ = data_.popPosition_ + direct3_ * 0.3f;
	world_.rotate_.y = GetYRotate(data_.direction_);
	colliderWorld_.parent_ = &world_;
	colliderWorld_.rotate_.y = 3.14f;

	// 反射板を残す時間
	aliveFrame_ = 20;
	//colliderWorld_.translate_.y = 1.0f;

	isActive_ = true;
	//collider_->SetColor({ 0.0f,0.0f,0.0f,1.0f });
}
