#include "AreaAttack.h"

#include "SphereCollider/SphereCollider.h"
#include "ImGuiManager/ImGuiManager.h"
#include"Effect/EffectSphereExplo/ESphereExplo.h"

void AreaAttack::Initialize(const Infomation& info)
{
	InstancingGameObject::Initialize("sphere");

	collider_.reset(new SphereCollider);
	collider_->Initialize("area", colliderWorld_);

	data_ = info;
	data_.power_ = 1.0f;
	momentFrame_ = 20;

	world_.Initialize();
	colliderWorld_.Initialize();
	world_.translate_ = data_.popPosition_;
	colliderWorld_.parent_ = &world_;

	//colliderWorld_.translate_.y = 1.0f;

	collider_->SetRadius(data_.power_);

	isActive_ = true;
	reqBehavior_ = SETUP;
	//collider_->SetColor({ 0.0f,0.0f,0.0f,1.0f });
}

void AreaAttack::Update()
{
	if (!isActive_)
	{
		return;
	}
	if (reqBehavior_)
	{
		behavior_ = reqBehavior_.value();
		switch (behavior_)
		{
		case AreaAttack::SETUP:
			momentFrame_ = 60;
			colliderWorld_.translate_.y = -100.0f;
			break;
		case AreaAttack::OUTBREAK:
			momentFrame_ = 5;
			colliderWorld_.translate_.y = 0.0f;
			SetExplo();
			break;
		case AreaAttack::SUSTAIN:
			momentFrame_ = 20;
			break;
		default:
			break;
		}
		reqBehavior_ = std::nullopt;
	}
	switch (behavior_)
	{
	case AreaAttack::SETUP:
		momentFrame_--;
		if (momentFrame_ <= 0)
		{
			reqBehavior_ = OUTBREAK;
		}
		//行列更新
		world_.UpdateMatrix();
		colliderWorld_.UpdateMatrix();
		collider_->Update();
		//sCollider_->Update();
		//タグに対応したモデルにワールド追加
		IMM_->SetData(tag_, colliderWorld_);
		break;
	case AreaAttack::OUTBREAK:
		momentFrame_--;
		if (momentFrame_ <= 0)
		{
			reqBehavior_ = SUSTAIN;
		}
		//行列更新
		world_.UpdateMatrix();
		colliderWorld_.UpdateMatrix();
		collider_->Update();
		//sCollider_->Update();
		//タグに対応したモデルにワールド追加
		//IMM_->SetData(tag_, colliderWorld_);
		break;
	case AreaAttack::SUSTAIN:
		momentFrame_--;
		if (momentFrame_ <= 0)
		{
			color = { 0.0f,0.0f,0.0f,0.0f };
			isActive_ = false;
		}
		// 発生とか持続の時にスケール変える？
		colliderWorld_.scale_ += Vector3{1.0f, 1.0f, 1.0f} * 0.5f;
		//行列更新
		world_.UpdateMatrix();
		colliderWorld_.UpdateMatrix();
		collider_->Update();
		//sCollider_->Update();
		//タグに対応したモデルにワールド追加
		//IMM_->SetData(tag_, colliderWorld_);
		break;
	default:
		break;
	}
}

void AreaAttack::Draw()
{
	if (!isActive_)
	{
		return;
	}
	collider_->Draw();
}

void AreaAttack::DebugWindow()
{
	collider_->Debug("area");

	ImGui::Begin(tag_.c_str());

	ImGui::Text("tra : %.2f,%.2f", world_.translate_.x, world_.translate_.z);
	ImGui::Separator();

	ImGui::End();
}

void AreaAttack::OnCollision()
{
}

void AreaAttack::SetExplo()
{
	ExploData data;
	data.world.translate_ = world_.GetMatWorldTranslate();
	data.maxDeadCount = 30;
	data.minScale = 0.0f;
	data.maxScale = 12.5f;
	data.maxScaleCount = 10;
	data.minAlphaCount = 15;

	EfSphereExplosion::GetInstance()->AddEffectData(data);
}