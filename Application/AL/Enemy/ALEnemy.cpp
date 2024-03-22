#include"AL/Enemy/ALEnemy.h"

#include"RandomNum/RandomNum.h"
#include"TextureManager/TextureManager.h"
#include"AudioManager/AudioManager.h"
#include"AL/BrokenBody/BrokenBody.h"

void ALEnemy::Initialize(const Vector3& position, const WorldTransform* playerWorld) {
	InstancingGameObject::Initialize("player");


	//model_->IsEnableTexture(false);
	//model_->SetBlendMode(BlendMode::kNone);
	world_.translate_ = position;

	tHeight = world_.translate_.y;

	playerWorld_ = playerWorld;
	//idou
	moveSPD_ = RandomNumber::Get(minSPD_, maxSPD_);

	world_.rotate_.y = RandomNumber::Get(0, 3.14f);

	shadow = std::make_unique<InstancingGameObject>();
	shadow->Initialize("DZone");
	
	shadow->SetColor({ 0,0,0,1 });
	shadow->SetTranslate({ world_.translate_.x,0,world_.translate_.z });
	shadow->SetScale(1.5f);

	mWorlds[HEAD].parent_ = (&world_);
	mWorlds[LARM].parent_ = (&mWorlds[HEAD]);
	mWorlds[RARM].parent_ = (&mWorlds[HEAD]);
	mWorlds[LFOOT].parent_ = (&mWorlds[HEAD]);
	mWorlds[RFOOT].parent_ = (&mWorlds[HEAD]);

	mWorlds[LFOOT].translate_ = { -0.5f,-0.8f,0 };
	mWorlds[RFOOT].translate_ = { 0.5f,-0.8f,0 };

	collider_ = std::make_unique<SphereCollider>();
	collider_->Initialize("ene", world_);
	collider_->SetRadius(1.5f);


	walkData_.RoopFrame = 10;
	walkData_.stPartsWorlds[HEAD].rotate_ = { 0.1f,0.0f,0.0f };
	walkData_.stPartsWorlds[LARM].rotate_ = { 0.0f,0.0f,-0.2f };
	walkData_.stPartsWorlds[RARM].rotate_ = { 0.0f,0.0f,0.2f };
	walkData_.stPartsWorlds[LFOOT].rotate_ = { -0.46f,0.0f,0.0f };
	walkData_.stPartsWorlds[RFOOT].rotate_ = { 0.2f,0.0f,0.0f };

	walkData_.edPartsWorlds[HEAD].rotate_ = { 0.1f,0.0f,0.0f };
	walkData_.edPartsWorlds[LARM].rotate_ = { 0.0f,0.0f,0.2f };
	walkData_.edPartsWorlds[RARM].rotate_ = { 0.0f,0.0f,-0.2f };
	walkData_.edPartsWorlds[LFOOT].rotate_ = { 0.2f,0.0f,0.0f };
	walkData_.edPartsWorlds[RFOOT].rotate_ = { -0.46f,0.0f,0.0f };

	stopData_.RoopFrame = 60;
	stopData_.stPartsWorlds[HEAD].rotate_ = { -0.2f,0.0f,0.0f };
	stopData_.stPartsWorlds[LARM].rotate_ = { 0.0f,0.0f,0.2f };
	stopData_.stPartsWorlds[RARM].rotate_ = { 0.0f,0.0f,-0.2f };
	stopData_.stPartsWorlds[LFOOT].rotate_ = { 0.23f,0.0f,0.0f };
	stopData_.stPartsWorlds[RFOOT].rotate_ = { 0.23f,0.0f,0.0f };

	stopData_.edPartsWorlds[HEAD].rotate_ = { 0.05f,0.0f,0.0f };
	stopData_.edPartsWorlds[LARM].rotate_ = { 0.0f,0.0f,0.3f };
	stopData_.edPartsWorlds[RARM].rotate_ = { 0.0f,0.0f,-0.3f };
	stopData_.edPartsWorlds[LFOOT].rotate_ = { -0.05f,0.0f,0.0f };
	stopData_.edPartsWorlds[RFOOT].rotate_ = { -0.05f,0.0f,0.0f };

	breakSound_ = AudioManager::LoadSoundNum("break");
}

void ALEnemy::Update() {

	isHit_ = false;

	if (state_ == Normal) {
		//pk
		Vector3 p_eVelo = playerWorld_->GetMatWorldTranslate() - world_.GetMatWorldTranslate();

		float p_eLength = Length(playerWorld_->GetMatWorldTranslate() - world_.GetMatWorldTranslate());

		//hanigai
		if (p_eLength > stopRange_ && p_eLength < serchRange_) {

			//プレイヤーの方向に移動
			Vector3 moveVelo{};
			moveVelo = p_eVelo;
			//ノーマライズ
			moveVelo.SetNormalize();
			//移動領分書ける
			moveVelo *= moveSPD_;
			//割る
			moveVelo /= maxSPDFrame;
			//速度に追加
			velocity_ += moveVelo;
			//速度ベクトルの量を取得
			float veloSPD = velocity_.GetLength();
			//プレイヤーへの向きベクトルに書ける
			velocity_ = p_eVelo.SetNormalize() * veloSPD;

			//最大速度に達していたら移動量もどす
			float spd = Length(velocity_);
			if (spd > moveSPD_) {
				velocity_.SetNormalize();
				velocity_ *= moveSPD_;
			}

			//Yいらない
			velocity_.y = 0;

			//加算処理
			world_.translate_ += velocity_;

			//muki
			if (moveVelo != Vector3(0, 0, 0)) {
				world_.rotate_.y = GetYRotate({ moveVelo.x,moveVelo.z });
			}

		}
		else {
			velocity_.SetZero();
		}

#pragma region モデルアニメーション
		//ベクトル量ゼロでアニメーション
		if (velocity_ == Vector3(0, 0, 0)) {

			if (moveState_ != StopS) {
				moveState_ = StopS;

				nowRoop_ = stopData_;
				std::swap(nowRoop_.stPartsWorlds, nowRoop_.edPartsWorlds);
				roopCount_ = 0;
			}
		}
		else {

			if (moveState_ != MoveS) {
				moveState_ = MoveS;
				roopCount_ = 0;
				nowRoop_ = walkData_;
			}


		}



		//ループ

		roopCount_++;

		float t = (float)roopCount_ / (float)nowRoop_.RoopFrame;


		int Index = 0;
		for (auto& world : mWorlds) {
			world.rotate_ = Esing(nowRoop_.stPartsWorlds[Index].rotate_, nowRoop_.edPartsWorlds[Index].rotate_, t);
			Index++;
		}

		//条件達成
		if (roopCount_ >= nowRoop_.RoopFrame) {
			t = 1;
			int Index = 0;
			for (auto& world : mWorlds) {
				world.rotate_ = Esing(nowRoop_.stPartsWorlds[Index].rotate_, nowRoop_.edPartsWorlds[Index].rotate_, t);
				Index++;
			}
			roopCount_ = 0;
			//値の入れ替え
			std::swap(nowRoop_.stPartsWorlds, nowRoop_.edPartsWorlds);
		}

#pragma endregion

	}

	//ヒット時モーション
	if (state_ == Hit) {

		velocity_ += acce;

		world_.translate_ += velocity_;

		world_.rotate_.x -= 0.5f;

		//高さがゼロ以下
		if (world_.translate_.y <= tHeight) {

			//HPがあるなら復帰
			if (HP_ > 0) {
				world_.translate_.y = tHeight;
				world_.rotate_.x = 0;
				state_ = Normal;
				velocity_.SetZero();

			}
			else {
				isDead_ = true;
				BrokenBody* BB = BrokenBody::GetInstance();
				BB->EffectOccurred(world_, 10);

				//音発生
				AudioManager::PlaySoundData(breakSound_, 0.2f);
			}

		}
	}


	//InstancingGameObject::Update();
	world_.UpdateMatrix();

	int IndexX = 0;
	for (auto& world : mWorlds) {
		world.UpdateMatrix();
		IndexX++;
	}



	collider_->Update();

	shadow->SetTranslate({ world_.translate_.x,0.1f,world_.translate_.z });
	shadow->Update();
}



bool ALEnemy::Collision(SphereCollider* collider) {

	Vector3 backVec;
	if (collider_->IsCollision(collider, backVec)) {

		state_ = Hit;

		velocity_ = hitVelo;

		Vector3 myPos = world_.GetMatWorldTranslate();
		Vector3 pPos = playerWorld_->GetMatWorldTranslate();

		myPos.y = 0;
		pPos.y = 0;

		Vector3 direc = myPos - pPos;
		direc.SetNormalize();
		direc *= hitSPD_;

		velocity_.x = direc.x;
		velocity_.z = direc.z;

		HP_--;

		return true;
	}

	return false;
}

Vector3 ALEnemy::OshiDashi(SphereCollider* collider)
{
	isHit_ = true;

	Vector3 backVec;
	if (collider_->IsCollision(collider, backVec)) {
		backVec.y = 0;
		backVec /= 2;
		world_.translate_ += backVec;

	}

	return backVec;
}

void ALEnemy::Draw() {

	world_.UpdateMatrix();
	int IndexX = 0;
	for (auto& world : mWorlds) {
		world.UpdateMatrix();
		IMM_->SetData(tags[IndexX], world);
		IndexX++;
	}

	shadow->Draw();
	//collider_->Draw();
}