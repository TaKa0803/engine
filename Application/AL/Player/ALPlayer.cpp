#include"ALPlayer.h"

#include"AudioManager/AudioManager.h"
#include"TextureManager/TextureManager.h"
#include<imgui.h>
#include<json.hpp>
#include<cassert>
#include<fstream>

ATKData LoadATKData(nlohmann::json::iterator& itGroup) {
	ATKData data;

	for (nlohmann::json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {

		//アイテム名
		const std::string& itemName = itItem.key();

		//各データ
		if (itemName == "data") {
			data.extraTime = (float)itItem->at(0);
			data.AttackTime = (float)itItem->at(1);
			data.RigorTime = (float)itItem->at(2);
			data.isYATK = (int)itItem->at(3);
			data.spd = (float)itItem->at(4);
		}
		else if (itemName == "ATKDerivation") {
			//皇族データ保存
			data.ATKDerivation.push_back(LoadATKData(itItem));
		}
	}

	return data;
}

void ALPlayer::LoadATKDatas() {

	//読み込み用ファイルストリーム
	std::ifstream ifs;
	//ファイルを読み込み用に開く
	ifs.open(atkDataPass_);

	//読み込み失敗
	if (ifs.fail()) {
		assert(false);
		return;
	}

	nlohmann::json root;
	//json文字列からjsonのデータ構造に展開
	ifs >> root;
	//移したのでファイルを閉じる
	ifs.close();




	int Index = 0;
	//攻撃グループ複数読み込み

		//グループ検索
	nlohmann::json::iterator itGroup = root.find(groupName_);

	//未登録チェック
	assert(itGroup != root.end());


	startATKData_ = LoadATKData(itGroup);



	Index++;

}



ALPlayer::ALPlayer() {
	//一回しかしない初期化情報
	input_ = Input::GetInstance();

	input_->SetDeadLine(0.3f);

	collider_ = std::make_unique<SphereCollider>();
	collider_->Initialize("player", world_);
	collider_->SetRadius(1.5f);

	//攻撃データの初期化
	LoadATKDatas();

	//GameObject::Initialize("player");

	int Index = 0;
	for (auto& tag : tags) {
		models[Index].reset(Model::CreateFromOBJ(tag));
		Index++;
	}

	textureData = TextureManager::LoadTex("resources/AppResource/Object/player.png");

	impactE_ = std::make_unique<EffectImpact>();

	shadow = std::make_unique<InstancingGameObject>();

	mWorlds[HEAD].parent_ = (&world_);
	mWorlds[LARM].parent_ = (&mWorlds[HEAD]);
	mWorlds[RARM].parent_ = (&mWorlds[HEAD]);
	mWorlds[LFOOT].parent_ = (&mWorlds[HEAD]);
	mWorlds[RFOOT].parent_ = (&mWorlds[HEAD]);


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


	ATKData1_.stPartsWorlds[HEAD].rotate_ = { -0.1f,0.0f,0.0f };
	ATKData1_.stPartsWorlds[LARM].rotate_ = { 0.0f,-0.4f,0.0f };
	ATKData1_.stPartsWorlds[RARM].rotate_ = { 0.0f,-0.7f,0.0f };
	ATKData1_.stPartsWorlds[LFOOT].rotate_ = { 0.1f,-0.6f,0.0f };
	ATKData1_.stPartsWorlds[RFOOT].rotate_ = { 0.1f,0.0f,0.0f };

	ATKData1_.edPartsWorlds[HEAD].rotate_ = { 0.3f,0.0f,0.0f };
	ATKData1_.edPartsWorlds[LARM].rotate_ = { 0.0f,0.83f,0.0f };
	ATKData1_.edPartsWorlds[RARM].rotate_ = { 0.0f,0.4f,0.0f };
	ATKData1_.edPartsWorlds[LFOOT].rotate_ = { 0.0f,-0.2f,0.0f };
	ATKData1_.edPartsWorlds[RFOOT].rotate_ = { 0.0f,0.1f,0.0f };

	for (int Index = 0; Index < modelNum_; ++Index) {
		ATKData2_.stPartsWorlds[Index] = ATKData1_.edPartsWorlds[Index];
	}

	ATKData2_.edPartsWorlds[HEAD].rotate_ = { -0.3f,0.0f,0.0f };
	ATKData2_.edPartsWorlds[LARM].rotate_ = { 0.0f,-0.5f,0.0f };
	ATKData2_.edPartsWorlds[RARM].rotate_ = { 0.0f,0.5f,0.0f };
	ATKData2_.edPartsWorlds[LFOOT].rotate_ = { -0.862f,0.0f,0.0f };
	ATKData2_.edPartsWorlds[RFOOT].rotate_ = { 0.3f,0.0f,0.0f };



	ATKData3_.stPartsWorlds[HEAD].rotate_ = { 1.5f,0.0f,0.0f };
	ATKData3_.stPartsWorlds[LARM].rotate_ = { 0.0f,0.0f,0.0f };
	ATKData3_.stPartsWorlds[RARM].rotate_ = { 0.0f,0.0f,0.0f };
	ATKData3_.stPartsWorlds[LFOOT].rotate_ = { 0.5f,0.0f,0.0f };
	ATKData3_.stPartsWorlds[RFOOT].rotate_ = { 0.5f,0.0f,0.0f };


	ATKData3_.edPartsWorlds[HEAD].rotate_ = { 1.5f,0.0f,15.0f };
	ATKData3_.edPartsWorlds[LARM].rotate_ = { 0.0f,0.0f,0.0f };
	ATKData3_.edPartsWorlds[RARM].rotate_ = { 0.0f,0.0f,0.0f };
	ATKData3_.edPartsWorlds[LFOOT].rotate_ = { 0.5f,0.0f,0.0f };
	ATKData3_.edPartsWorlds[RFOOT].rotate_ = { 0.5f,0.0f,0.0f };


	startATKData_.parts = ATKData1_;
	startATKData_.ATKDerivation[0].parts = ATKData2_;
	startATKData_.ATKDerivation[0].ATKDerivation[0].parts = ATKData3_;

	punchSound_ = AudioManager::LoadSoundNum("com1");
	kickSound_ = AudioManager::LoadSoundNum("com2");
	drilSound_ = AudioManager::LoadSoundNum("com3");

	
}

ALPlayer::~ALPlayer() {

}

void ALPlayer::Initialize() {
	//中身データ初期化
	world_.Initialize();

	world_.translate_.y = 1.5f;

	mWorlds[LFOOT].translate_ = { -0.5f,-0.8f,0 };
	mWorlds[RFOOT].translate_ = { 0.5f,-0.8f,0 };


	roopState = SetUp;
	roopCount_ = 0;

	moveState_ = NoneS;

	ATKConboCount = 1;
	ATKAnimationSetup_ = false;

	shadow->Initialize("DZone");
	shadow->SetParent(&world_);
	shadow->SetColor({ 0,0,0,1 });
	shadow->SetTranslate({ 0,-1.45f,0 });
	shadow->SetScale(1.5f);

	impactE_->Initialize();
}

void ALPlayer::Update() {


	//状態の初期化処理
	if (stateRequest_) {
		state_ = stateRequest_.value();
		stateRequest_ = std::nullopt;

		//実際の初期化処理
		(this->*StateInitialize[(int)state_])();
	}

	//状態の更新
	(this->*StateUpdate[(int)state_])();

	//行列更新

	world_.UpdateMatrix();

	int Index = 0;
	for (auto& world : mWorlds) {
		world.UpdateMatrix();
	}

	collider_->Update();

	shadow->Update();

	impactE_->Update();
}

void (ALPlayer::* ALPlayer::StateInitialize[])() = {
	&ALPlayer::InitializeMove,
	&ALPlayer::InitializeATK,
	&ALPlayer::InitializeHitAction,
	&ALPlayer::InitializeSpecialATK
};

void (ALPlayer::* ALPlayer::StateUpdate[])() = {
	&ALPlayer::UpdateMove,
	&ALPlayer::UpdateATK,
	&ALPlayer::UpdateHitAction,
	&ALPlayer::InitializeSpecialATK
};

void ALPlayer::Draw() {
	//IGameObject::Draw(viewprojection);
	//model_->Draw(world_.matWorld_, viewprojection, TextureManager::uvChecker_);


	//各モデル描画
	int Index = 0;
	for (auto& model : models) {
		model->Draw(mWorlds[Index].matWorld_, *camera_, {0,0,0}, textureData);

		Index++;
	}

	shadow->Draw();

	impactE_->Draw();

	//collider_->Draw();
}

void ALPlayer::DebugWindow(const char* name) {

	float cScale = collider_->GetRadius();

	ImGui::Begin(name);
	world_.DrawDebug(name);

	mWorlds[HEAD].DrawDebug("head");
	mWorlds[LARM].DrawDebug("LA");
	mWorlds[RARM].DrawDebug("RA");
	mWorlds[LFOOT].DrawDebug("LF");
	mWorlds[RFOOT].DrawDebug("RF");

	ImGui::DragFloat("collider scale", &cScale, 0.1f, 1, 10);

	//model_->DebugParameter(name);
	ImGui::End();

	collider_->SetRadius(cScale);
}



void ALPlayer::Move() {

	Vector3 move{};

	//移動取得
	move = input_->GetWASD();

	if (input_->IsControllerActive()) {
		move = input_->GetjoyStickLV3();
	}

	move.SetNormalize();
	move *= spd_;

	//カメラ方向に向ける
	move = TransformNormal(move, camera_->GetMainCamera().matWorld_);

	move.y = 0;

	if (move != Vector3(0, 0, 0)) {
		world_.rotate_.y = GetYRotate({ move.x,move.z });
	}
	//加算
	world_.translate_ += move;


	ModelRoop(move);
}





void ALPlayer::ModelRoop(const Vector3& velo) {

	//ベクトル量ゼロでアニメーション
	if (velo == Vector3(0, 0, 0)) {

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



}

#pragma region 各状態初期化処理


void ALPlayer::InitializeMove() {

	nowRoop_ = stopData_;
	moveState_ = StopS;
	std::swap(nowRoop_.stPartsWorlds, nowRoop_.edPartsWorlds);
	roopCount_ = 0;
}

void ALPlayer::InitializeATK() {

	atkState_ = ATKState::Extra;

	updateATKData_ = ATKUpdateData{};

	//どちらのボタンが最初かで攻撃ツリー変更

	ATKData_ = startATKData_;

	ATKAnimationSetup_ = false;

	ATKConboCount = 1;
}

void ALPlayer::InitializeHitAction() {

}

void ALPlayer::InitializeSpecialATK() {
}

void ALPlayer::UpdateSpecialATK() {
}

#pragma endregion

#pragma region 各状態更新処理

void ALPlayer::UpdateMove() {
	//移動処理
	Move();

	//攻撃状態に移るかの処理
	bool isATK = false;

	//スペシャル攻撃
	bool isSpecialATK = false;


	isATK = input_->TriggerKey(DIK_Z);

	isSpecialATK = input_->TriggerKey(DIK_C);


	//コントローラーがあるときの処理
	if (input_->IsControllerActive()) {

		isATK = input_->IsTriggerButton(kButtonB);

		isSpecialATK = input_->IsTriggerButton(kButtonY);

	}

	if (isATK) {
		stateRequest_ = State::ATK;
	}

}

void ALPlayer::UpdateATK() {


#pragma region 実行処理
	if (atkState_ == ATKState::Extra) {

		//初期設定
		if (!ATKAnimationSetup_) {
			ATKAnimationSetup_ = true;

			for (int partsNum = 0; partsNum < modelNum_; ++partsNum) {
				nowRoop_.stPartsWorlds[partsNum] = mWorlds[partsNum];
				nowRoop_.edPartsWorlds[partsNum] = ATKData_.parts.stPartsWorlds[partsNum];
			}
		}
		else {





			float t = (float)updateATKData_.count / (float)ATKData_.extraTime;


			int Index = 0;
			for (auto& world : mWorlds) {
				world.rotate_ = Esing(nowRoop_.stPartsWorlds[Index].rotate_, nowRoop_.edPartsWorlds[Index].rotate_, t);
				Index++;
			}


			updateATKData_.count++;
			//条件を満たしたら次の状態へ
			if (updateATKData_.count >= ATKData_.extraTime) {
				t = 1;
				int Index = 0;
				for (auto& world : mWorlds) {
					world.rotate_ = Esing(nowRoop_.stPartsWorlds[Index].rotate_, nowRoop_.edPartsWorlds[Index].rotate_, t);
					Index++;
				}

				atkState_ = ATKState::ATK;
				updateATKData_.count = 0;
				ATKAnimationSetup_ = false;
			}

		}
	}

	if (atkState_ == ATKState::ATK) {
		if (!ATKAnimationSetup_) {
			ATKAnimationSetup_ = true;
			for (int partsNum = 0; partsNum < modelNum_; ++partsNum) {
				nowRoop_.stPartsWorlds[partsNum] = mWorlds[partsNum];
				nowRoop_.edPartsWorlds[partsNum] = ATKData_.parts.edPartsWorlds[partsNum];

				if (ATKConboCount == 1) {
					AudioManager::PlaySoundData(punchSound_,0.05f);
				}
				else if (ATKConboCount == 2) {
					AudioManager::PlaySoundData(kickSound_,0.05f);
				}
				else if (ATKConboCount == 3) {
					AudioManager::PlaySoundData(drilSound_,0.05f);
				}
			}

			impactE_->Spawn(world_);
		}
		else {

			//予備動作中向き変更
			Vector3 move{};
			//データ
			move = input_->GetWASD();
			if (input_->IsControllerActive()) {
				move = input_->GetjoyStickLV3();
			}

			move.SetNormalize();
			//カメラ方向に向ける
			move = TransformNormal(move, camera_->GetMainCamera().matWorld_);
			move.y = 0;

			if (move != Vector3(0, 0, 0)) {
				world_.rotate_.y = GetYRotate({ move.x,move.z });

				move *= ATKData_.spd;

				world_.translate_ += move;
			}
			else {
				Vector3 offset = { 0,0,1 };
				offset = TransformNormal(offset, camera_->GetMainCamera().matWorld_);

				world_.rotate_.y = GetYRotate({ offset.x,offset.z });

				offset *= ATKData_.spd;

				offset.y = 0;

				world_.translate_ += offset;
			}


			float t = (float)updateATKData_.count / (float)ATKData_.AttackTime;

			int Index = 0;
			for (auto& world : mWorlds) {
				world.rotate_ = Esing(nowRoop_.stPartsWorlds[Index].rotate_, nowRoop_.edPartsWorlds[Index].rotate_, t);
				Index++;
			}

			updateATKData_.count++;
			//条件を満たしたら次の状態へ
			if (updateATKData_.count >= ATKData_.AttackTime) {
				t = 1;
				int Index = 0;
				for (auto& world : mWorlds) {
					world.rotate_ = Esing(nowRoop_.stPartsWorlds[Index].rotate_, nowRoop_.edPartsWorlds[Index].rotate_, t);
					Index++;
				}

				atkState_ = ATKState::Rigor;
				updateATKData_.count = 0;
				ATKAnimationSetup_ = false;
			}

		}
	}

	if (atkState_ == ATKState::Rigor) {
		updateATKData_.count++;
		//条件を満たしたら次の状態へ
		if (updateATKData_.count >= ATKData_.RigorTime) {
#pragma region 条件によるシーン転換

			if (ATKConboCount == 1) {
				AudioManager::StopSound(punchSound_);
			}
			else if (ATKConboCount == 2) {
				AudioManager::StopSound(kickSound_);
			}
			else if (ATKConboCount == 3) {
				AudioManager::StopSound(drilSound_);
			}


			//攻撃入力フラグON
			if (updateATKData_.nextATK && ATKData_.ATKDerivation.size() != 0) {

				ATKData_ = ATKData_.ATKDerivation[0];

				updateATKData_ = ATKUpdateData{};
				ATKAnimationSetup_ = false;
				atkState_ = ATKState::Extra;

				ATKConboCount++;
			}
			else {
				//移動状態に変更
				stateRequest_ = State::Move;
				ATKConboCount = 0;
			}
#pragma endregion

		}
	}
#pragma endregion

#pragma region 実行中のキー入力受付

	if (input_->IsTriggerButton(kButtonB) || input_->TriggerKey(DIK_Z)) {
		updateATKData_.nextATK = true;
	}
#pragma endregion




}

void ALPlayer::UpdateHitAction() {

}

#pragma endregion