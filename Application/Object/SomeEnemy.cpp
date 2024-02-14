#include "SomeEnemy.h"

#include "ImGuiManager/ImGuiManager.h"
#include "RandomNum/RandomNum.h"
#include"Effect/EffectSphereExplo/ESphereExplo.h"

void SomeEnemy::Initialize(const WorldTransform& world)
{
	InstancingGameObject::Initialize("parrot");
	world_.Initialize();
	world_.scale_ = { 1.0f,1.0f,1.0f };
	//world_.translate_.y = 1.0f;
	world_.UpdateMatrix();

	collider_.reset(new SphereCollider);
	collider_->Initialize("enemy");

	isActive_ = true;
	isHit_ = false;
	isFactor_ = false;

	playerW_ = &world;

	//初期化
	reqBehavior_ = IDOL;

	momentFrame_ = cALIVEFRAME_ + (int)RandomNumber::Get(0.0f, 10.0f);

	//各データ待ち時間
	atkCount_[wait].maxCount = 60;
	atkCount_[atk].maxCount = 15;
	atkCount_[stop].maxCount = 60;
	atkCount_[back].maxCount = 60;

}

void SomeEnemy::Update()
{
	//死んでいたら早期リターン
	if (!isActive_) {
		return;
	}

	//共通処理
	//if (behavior_ != BURST) {
		momentFrame_--;
		if (momentFrame_ <= 0)
		{
			isActive_ = false;
		}
	//}

	//雑魚敵のタイプによる処理
	switch (eType_)
	{
	case SomeEnemy::Move:
		//動く敵の処理
		MoveEnemyUpdate();
		break;
	case SomeEnemy::Explo:
		//当たったら即爆するやつ
		ExpEnemyUpdate();
		break;
	default:
		break;
	}

	world_.UpdateMatrix();
	collider_->SetTranslate(world_.translate_);
	collider_->Update();
}

void SomeEnemy::DebagWindow(int num)
{
	collider_->Debug("enemy");

	ImGui::Begin("SomeEnemy");

	ImGui::DragFloat3(std::to_string(num).c_str(), &world_.translate_.x, 0.02f);

	ImGui::End();
}

void SomeEnemy::Draw()
{
	if (!isActive_) {
		return;
	}
	//collider_->Draw();
	IMM_->SetData(tag_, world_);
}

void SomeEnemy::OnCollision(const Vector3& direction)
{
	if (behavior_ == DESTROY)
	{
		return;
	}
	if (behavior_ == BURST)
	{
		world_.translate_ += direction;
		direct3_ = direction;
		direct3_.y = 0;
		direct3_.SetNormalize();
		direct3_ *= burstSpd_;
	}

	// 一度だけバースト初期化
	else if (eType_ == Move) {
		direct3_ = direction;
		direct3_.y = 0;
		direct3_.SetNormalize();
		direct3_ *= burstSpd_;
		reqBehavior_ = BURST;
	}//
	else if (eType_ == Explo)
	{
		reqBehavior_ = DESTROY;
	}

	world_.UpdateMatrix();
	collider_->Update();
}

void SomeEnemy::MoveEnemyUpdate() {
#pragma region 動く敵の処理
	if (reqBehavior_)
	{
		behavior_ = reqBehavior_.value();
		switch (behavior_)
		{
		case SomeEnemy::IDOL:

			break;
		case SomeEnemy::MOVE:
			//momentFrame_ = cALIVEFRAME_;
			break;
		case SomeEnemy::BURST:
			momentFrame_ = cBURSTFRAME_;
			//isHit_ = false;
			break;
		case SomeEnemy::DESTROY:
			momentFrame_ = 60;
			SetExplo();
			break;
		case SomeEnemy::ATK:
			//全部初期化
			for (auto& cont : atkCount_) {
				cont.count = 0;
				cont.initialize = false;
			}
			break;

		default:
			break;
		}
		reqBehavior_ = std::nullopt;
	}

	switch (behavior_)
	{
	case SomeEnemy::IDOL:

		IDOLUpdate();



		break;
	case SomeEnemy::MOVE:

		//移動処理
		MoveToPlayer();

		break;
	case SomeEnemy::BURST:
		momentFrame_--;
		if (momentFrame_ <= 0)
		{
			reqBehavior_ = DESTROY;
		}
		world_.translate_ += direct3_ * 0.5f;
		world_.rotate_.y = GetYRotate({ direct3_.x,direct3_.z });
		break;
	case SomeEnemy::DESTROY:
		momentFrame_--;
		if (momentFrame_ <= 0)
		{
			isActive_ = false;
		}
		collider_->SetRadius(1.5f + (1.0f - momentFrame_ / 60.0f));
		break;

	case SomeEnemy::ATK:

		ATKToPlayerUpdate();

		break;
	}

#pragma endregion
}

void SomeEnemy::ExpEnemyUpdate()
{
#pragma region ��������G
	if (reqBehavior_)
	{
		behavior_ = reqBehavior_.value();
		switch (behavior_)
		{
		case SomeEnemy::IDOL:
			momentFrame_ = cALIVEFRAME_;
			break;
		case SomeEnemy::MOVE:
			momentFrame_ = cALIVEFRAME_;
			break;
		case SomeEnemy::BURST:
			momentFrame_ = cBURSTFRAME_;
			break;
		case SomeEnemy::DESTROY:
			momentFrame_ = 60;

			SetExplo();
			break;
		default:
			break;
		}
		reqBehavior_ = std::nullopt;
	}



	switch (behavior_)
	{
	case SomeEnemy::IDOL:

		break;
	case SomeEnemy::MOVE:

		break;
	case SomeEnemy::BURST:
		momentFrame_--;
		if (momentFrame_ <= 0)
		{
			reqBehavior_ = DESTROY;
		}
		world_.translate_ += direct3_ * 0.5f;
		break;
	case SomeEnemy::DESTROY:
		momentFrame_--;
		if (momentFrame_ <= 0)
		{
			isActive_ = false;
		}

		collider_->SetRadius(1.5f + (1.0f - momentFrame_ / 60.0f));
		break;
	default:
		break;
	}

#pragma endregion

}

void SomeEnemy::IDOLUpdate()
{
	//プレイヤーと離れていれば移動状態へ
	Vector3 moveVelo{};
	moveVelo = playerW_->GetMatWorldTranslate();

	Vector3 leng = world_.GetMatWorldTranslate() - moveVelo;
	if (Length(leng) >= nearPRadius_) {
		reqBehavior_ = MOVE;
	}
	else {
		//距離内なら攻撃状態へ
		reqBehavior_ = ATK;
	}



}

void SomeEnemy::MoveToPlayer()
{
#pragma region プレイヤーに寄る処理
	//プレイヤーの方向に移動
	Vector3 moveVelo{};
	moveVelo = playerW_->GetMatWorldTranslate();

	Vector3 leng = moveVelo - world_.GetMatWorldTranslate();

	if (Length(leng) >= nearPRadius_) {

		//ノーマライズ
		leng.SetNormalize();
		//移動領分書ける
		leng *= spd_;
		//割る

		leng.y = 0;

		//速度に追加
		world_.translate_ += leng;


		//muki
		if (leng != Vector3(0, 0, 0)) {
			world_.rotate_.y = GetYRotate({ leng.x,leng.z });
		}
	}
	else {
		reqBehavior_ = IDOL;
	}
#pragma endregion

}

void SomeEnemy::ATKToPlayerUpdate()
{
	//攻撃
	if (ATKStateCount_ == wait) {
		//プレイヤーの方向にmuku
		Vector3 moveVelo{};
		moveVelo = playerW_->GetMatWorldTranslate();

		Vector3 leng = moveVelo - world_.GetMatWorldTranslate();


		//muki
		if (leng != Vector3(0, 0, 0)) {
			world_.rotate_.y = GetYRotate({ leng.x,leng.z });
		}
	}

	//攻撃時の更新処理
	if (ATKStateCount_ == atk) {
		world_.translate_ += atkDirection_;
	}

	//カウント最大数で変化
	if (atkCount_[ATKStateCount_].count++ >= atkCount_[ATKStateCount_].maxCount) {

		//上限ではなければ処理
		if (ATKStateCount_ != back) {
			ATKStateCount_++;
			//先のデータ初期化
			atkCount_[ATKStateCount_].count = 0;
			atkCount_[ATKStateCount_].initialize = false;

			//過去データ初期化
			atkCount_[ATKStateCount_ - 1].initialize = false;
			atkCount_[ATKStateCount_ - 1].count = 0;

		}
		else {

			//待機状態に戻して初期化
			reqBehavior_ = IDOL;
		}

		//攻撃の初期化処理
		if (ATKStateCount_ == atk) {
			Vector3 moveVelo = playerW_->GetMatWorldTranslate();
			Vector3 pos = world_.GetMatWorldTranslate();

			Vector3 leng = moveVelo - pos;


			atkDirection_ = leng.SetNormalize() * atkSpd_;
		
		}

	}
}

void SomeEnemy::SetExplo()
{
	ExploData data;
	data.world.translate_ = world_.GetMatWorldTranslate();
	data.maxDeadCount = 60;
	data.minScale = 0.0f;
	data.maxScale = 2.5f;
	data.maxScaleCount = 60;
	data.minAlphaCount = 60 / 2;

	EfSphereExplosion::GetInstance()->AddEffectData(data);
}

void SomeEnemy::OnEnemy(const Vector3& direction)
{
	if (eType_ == Explo)
	{
		isFactor_ = true;
		reqBehavior_ = DESTROY;
	}
	else if (eType_ == Move)
	{
		isFactor_ = true;
		direct3_ = Normalize(direction);
		reqBehavior_ = BURST;
	}
}

