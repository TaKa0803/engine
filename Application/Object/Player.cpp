#include "Player.h"

#include <cmath>
#include "Camera/Camera.h"
#include"AudioManager/AudioManager.h"

Player::Player()
{
	GameObject::Initialize("talker");
	input_ = Input::GetInstance();
}

void Player::Initialize()
{
	world_.Initialize();
	world_.scale_ = { 1.0f,1.0f,1.0f };
	world_.translate_.y = 1.0f;
	world_.rotate_.y = 3.14f / 2.0f;
	model_->SetUVScale({ 1.0f,1.0f,1.0f });
	world_.UpdateMatrix();

	collider_.reset(new SphereCollider);
	collider_->Initialize("player", world_);

	blasts_.clear();
	isInvisible_ = true;
	invisibleFrame_ = 60;
	reqBehavior_ = IDOL;

	shotSound_ = AudioManager::LoadSoundNum("PlayerShot");
}

void Player::Update()
{
	blasts_.remove_if([](const std::unique_ptr<EchoBlast>& echo) {
		if (echo->GetIsActive())
		{
			return false;
		}
		return true;
		});

	if (isInvisible_)
	{
		invisibleFrame_--;
		if (invisibleFrame_ <= 0)
		{
			isInvisible_ = false;
		}
	}

	if (reqBehavior_)
	{
		behavior_ = reqBehavior_.value();
		switch (behavior_)
		{
		case Player::IDOL:
			break;
		case Player::ATTACK:
		{
			AudioManager::PlaySoundData(shotSound_);

			EchoBlast::Infomation info;
			info.mode_ = ATTACKMODE::aSPOT;
			info.direction_ = direction_;
			info.popPosition_ = world_.translate_;
			// クラス内で定義
			info.power_ = 1.0f;
			CreateEcho(info);
		}
		momentFrame_ = 5;
		break;
		case Player::MIRROR:
		{
			EchoBlast::Infomation info;
			info.mode_ = ATTACKMODE::aMIRROR;
			info.direction_ = direction_;
			info.popPosition_ = world_.translate_;
			// クラス内で定義
			info.power_ = 1.0f;
			CreateEcho(info);
		}
		momentFrame_ = 30;
		isEndMirror_ = false;
		break;
		case Player::MOMENT:
			momentFrame_ += 10;
			break;
		default:
			break;
		}
		reqBehavior_ = std::nullopt;
	}
	switch (behavior_)
	{
	case Player::IDOL:
		UpdateIDOL();
		break;
	case Player::ATTACK:
		UpdateATTACK();
		break;
	case Player::MIRROR:
		UpdateMIRROR();
		break;
	case Player::MOMENT:
		momentFrame_--;
		if (momentFrame_ <= 0)
		{
			reqBehavior_ = IDOL;
		}
		break;
	case Player::_COUNT:
		break;
	default:
		break;
	}

	world_.UpdateMatrix();
	collider_->Update();

	std::list<std::unique_ptr<EchoBlast>>::iterator itr = blasts_.begin();
	for (; itr != blasts_.end(); ++itr)
	{
		itr->get()->Update();
	}
}

void Player::DebagWindow()
{
	model_->DebugParameter("player");
	collider_->Debug("player");

	std::list<std::unique_ptr<EchoBlast>>::iterator itr = blasts_.begin();
	for (; itr != blasts_.end(); ++itr)
	{
		itr->get()->DebugWindow();
	}
}

void Player::Draw(const Camera& viewp)
{
	GameObject::Draw(viewp);
	collider_->Draw();
	std::list<std::unique_ptr<EchoBlast>>::iterator itr = blasts_.begin();
	for (; itr != blasts_.end(); ++itr)
	{
		itr->get()->Draw();
	}
}

bool Player::OnCollision()
{
	if (isInvisible_)
	{
		return false;
	}
	reqBehavior_ = MOMENT;
	momentFrame_ += 20;
	invisibleFrame_ = 140;
	isInvisible_ = true;
	return true;
}

void Player::UpdateIDOL()
{
	UpdateMove();


	if (input_->TriggerKey(DIK_LSHIFT) ||
		input_->IsTriggerButton(kRightTrigger))
	{
		reqBehavior_ = MIRROR;
	}
	//// モード切替
	//if (input_->TriggerKey(DIK_Q))
	//{
	//	mode_ = ATTACKMODE::aSPOT;
	//}
	//else if (input_->TriggerKey(DIK_E))
	//{
	//	mode_ = ATTACKMODE::aMIRROR;
	//}
	//if (input_->IsTriggerButton(kRightTrigger))
	//{
	//	switch (mode_)
	//	{
	//	case ATTACKMODE::aSPOT:
	//		mode_ = ATTACKMODE::aMIRROR;
	//		break;
	//	case ATTACKMODE::aMIRROR:
	//		mode_ = ATTACKMODE::aSPOT;
	//		break;
	//	default:
	//		break;
	//	}
	//}
	// 音発射
	else if (input_->TriggerKey(DIK_SPACE) ||
		input_->IsTriggerButton(kLeftTrigger))
	{
		reqBehavior_ = ATTACK;
	}
}

void Player::UpdateMove()
{
	Vector3 move = input_->GetWASD();
	if (input_->IsControllerActive())
	{
		if (move.GetLength() == 0.0f)
		{
			move = input_->GetjoyStickLV3();
		}
	}
	float speed = 0.3f;

	move.SetNormalize();
	move *= speed;

	move = TransformNormal(move, camera_->GetMainCamera().matWorld_);

	move.y = 0;

	if (move != Vector3(0, 0, 0))
	{
		world_.rotate_.y = GetYRotate({ move.x,move.z });
		direction_ = { std::sinf(world_.rotate_.y),std::cosf(world_.rotate_.y) };
		world_.rotate_.y += 3.14f / 2.0f;
		//reqBehavior_ = MOVE;
	}
	//加算
	world_.translate_ += move;
}

void Player::UpdateATTACK()
{
	momentFrame_--;
	if (momentFrame_ <= 0)
	{
		reqBehavior_ = MOMENT;
	}
}

void Player::UpdateMIRROR()
{
	// 押していない
	if (!input_->PushKey(DIK_LSHIFT) &&
		!input_->IsPushButton(kRightTrigger))
	{
		isEndMirror_ = true;
	}

	// 反射板終了動作のコードを書ける...?
	if(isEndMirror_)
	{
		momentFrame_--;
		if (momentFrame_ <= 0)
		{
			reqBehavior_ = MOMENT;
		}
		return;
	}

	Vector3 move = input_->GetWASD();
	if (input_->IsControllerActive())
	{
		if (move.GetLength() == 0.0f)
		{
			move = input_->GetjoyStickLV3();
		}
	}
	move.SetNormalize();

	move = TransformNormal(move, camera_->GetMainCamera().matWorld_);

	move.y = 0;

	if (move != Vector3(0, 0, 0))
	{
		world_.rotate_.y = GetYRotate({ move.x,move.z });
		direction_ = { std::sinf(world_.rotate_.y),std::cosf(world_.rotate_.y) };
		world_.rotate_.y += 3.14f / 2.0f;
		//reqBehavior_ = MOVE;
	}

	EchoBlast::Infomation info;
	info.mode_ = ATTACKMODE::aMIRROR;
	info.direction_ = direction_;
	info.popPosition_ = world_.translate_;
	// クラス内で定義
	info.power_ = 1.0f;
	blasts_.back()->SetInfomation(info);
	
}

void Player::CreateEcho(const EchoBlast::Infomation& info)
{
	EchoBlast* echo = new EchoBlast;
	echo->Initialize(info);
	blasts_.emplace_back(echo);
}
