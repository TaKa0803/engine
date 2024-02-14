#include "BossEnemy.h"
#include <random>
#include <time.h>

#include"Effect/EffectSphereExplo/ESphereExplo.h"
#include"Effect/dZoneE/DZone.h"
#include "GlobalVariables/GlobalVariables.h"
#include "ImGuiManager/ImGuiManager.h"
#include "SomeEnemy.h"
#include "AreaAttack.h"

BossEnemy::BossEnemy()
{
	GameObject::Initialize("Boss");

#pragma region HPゲージ関連
	//HPバー
	int tex = TextureManager::LoadTex(hpTex_);
	Vector2 pos = {
		958.4f,
		60
	};
	hpBar_.reset(Sprite::Create(tex, { 962,58 }, { 962,58 }, { 962,58 }, pos, { 1.0f,0.5f }));

	//HPバー背景
	tex = TextureManager::LoadTex(hpBackTex_);
	hpBarBack_.reset(Sprite::Create(tex, { 962,58 }, { 962,58 }, { 962,58 }, { 640,60 }));

	//フレーム画像
	tex = TextureManager::LoadTex(hpFrameTex_);
	hpBarFrame_.reset(Sprite::Create(tex, { 962,58 }, { 962,58 }, { 962,58 }, { 640,60 }));


	hpBar_->SetParent(uiWorld_);
	hpBarBack_->SetParent(uiWorld_);
	hpBarFrame_->SetParent(uiWorld_);

#pragma endregion


#pragma region ボス攻撃関連

	specialATK.ammo = std::make_unique<InstancingGameObject>();
	specialATK.ammo->Initialize("bomb");
	specialATK.collider = std::make_unique<SphereCollider>();

	specialATK.count[wait].maxCount = 60 * 1;
	specialATK.count[shotMove].maxCount = 30;
	specialATK.count[shotedWait].maxCount = 60 * 10;
	specialATK.count[back].maxCount = 30;
	specialATK.count[HitDown].maxCount = 60 * 10;


#pragma endregion



	srand((unsigned int)time(nullptr));
}

void BossEnemy::Initialize(const WorldTransform& player)
{
	world_.Initialize();
	world_.scale_ = { cSIZE_,cSIZE_,cSIZE_ };
	world_.translate_.z = 20.0f;
	world_.translate_.y = 2.0f;
	model_->SetUVScale({ 1.0f,1.0f,1.0f });

	world_.UpdateMatrix();

	collider_.reset(new SphereCollider);
	collider_->Initialize("boss", world_);
	collider_->SetRadius(cSIZE_);

	enemies_.clear();
	attacks_.clear();

	reqBehavior_ = IDOL;
	isActive_ = true;

	invisibleFrame_ = 60;
	isInvisible_ = false;

	damage_ = 0.0f;

	// 範囲取得
	moveLength_ = GlobalVariables::GetInstance()->GetFloatvalue("StageWall", "Size");
	moveCount_ = 0;

#pragma region HP関連初期化
	//HPの初期化
	HP_ = maxHP_;
	hpBar_->SetScale(hpBarScale);

	uiWorld_.translate_ = uiPos_;
	uiWorld_.scale_ = UIScale_;
#pragma endregion

#pragma region ボス必殺技関係
	for (auto& count : specialATK.count)
	{
		count.count = 0;
	}

	specialATK.isSpecialATK_ = false;
	specialATK.nextSpecialATKCount = 0;
	specialATK.stateCount = 0;
	specialATK.isHitPlayerWall = false;
	specialATK.isShot = false;

	specialATK.collider->Initialize("ammo", specialATK.ammo->GetWorld());
#pragma endregion


	//プレイヤーworld取得
	playerW_ = &player;

}

void BossEnemy::Update()
{
	if (!isActive_)
	{
		return;
	}
	if (behavior_ != CRUSH && HP_ <= 0)
	{
		reqBehavior_ = CRUSH;
	}

	//必殺技状態ではないときカウント増加
	if (behavior_ != SPECIAL)
	{
		specialATK.nextSpecialATKCount++;
	}

	enemies_.remove_if([](const std::unique_ptr<SomeEnemy>& some) {
		if (some->GetIsActive())
		{
			return false;
		}
		//エフェクトを生成
		ExploData data;
		data.world.translate_ = some->GetWorld().GetMatWorldTranslate();
		data.maxDeadCount = 60;
		data.minScale = 0.0f;
		data.maxScale = 2.5f;
		data.maxScaleCount = 60;
		data.minAlphaCount = 60 / 2;

		EfSphereExplosion::GetInstance()->AddEffectData(data);

		return true;
		});

	if (reqBehavior_)
	{
		behavior_ = reqBehavior_.value();
		switch (behavior_)
		{
		case BossEnemy::IDOL:
			momentFrame_ += 60;
			break;
		case BossEnemy::MOVE:
		{
			int length = (int)moveLength_;
			int rnd1 = length / 2 - rand() % length;
			int rnd2 = length / 2 - rand() % length;
			nextPosition_ = { (float)rnd1,(float)rnd2 };
			Vector3 pos = { nextPosition_.x,world_.translate_.y,nextPosition_.y };
			//エリア生成
			DZoneEData effect;
			effect.maxCount = float(cMOVEFRAME_ / 3);
			effect.tenmetu = 2;
			effect.isSphere = true;
			effect.pos.st = pos;
			effect.pos.ed = pos;

			effect.scale.st = { 10.0f,10.0f,10.0f };
			effect.scale.ed = { 10.0f,10.0f,10.0f };

			DZoneEffect::GetInstance()->AddData(effect);
		}
		momentFrame_ = cMOVEFRAME_;
		moveCount_++;
		attackCount_ = 0;
		stompCount_ = 0;
		break;
		case BossEnemy::SUMMON:
			SeePlayer();
			momentFrame_ = cSUMMONFRAME_;
			// 移動期待値 0
			moveCount_ = 0;
			attackCount_ = 0;
			stompCount_ = 0;
			break;
		case BossEnemy::ATTACK:
			SeePlayer();
			momentFrame_ = cATTACKFRAME_;
			// 移動期待値 0
			moveCount_ = 0;
			attackCount_++;
			stompCount_ = 0;
			break;
		case BossEnemy::STOMP:
			momentFrame_ = cSTOMPFRAME_;
			moveCount_ = 0;
			attackCount_ = 0;
			stompCount_++;
			break;
		case BossEnemy::DAMAGE:
			SetAplta(0.5f);
			momentFrame_ = 5;
			invisibleFrame_ = 10;
			isInvisible_ = true;
			HP_ -= (int)damage_;
			damage_ = 0.0f;

			for (auto& count : specialATK.count)
			{
				count.count = 0;
			}
			specialATK.isShot = 0;
			specialATK.nextSpecialATKCount = 0;
			specialATK.stateCount = 0;
			specialATK.isHitPlayerWall = false;
			break;
		case BossEnemy::CRUSH:
			SetAplta(0.5f);
			momentFrame_ = cCRUSHFRAME_;
			isInvisible_ = true;
			break;
		case BossEnemy::SPECIAL:

			SeePlayer();

			for (auto& count : specialATK.count)
			{
				count.count = 0;
			}
			specialATK.isShot = 0;
			specialATK.nextSpecialATKCount = 0;
			specialATK.stateCount = 0;
			specialATK.isHitPlayerWall = false;
			break;
		default:
			break;
		}
		reqBehavior_ = std::nullopt;
	}

	switch (behavior_)
	{
	case BossEnemy::IDOL:
		UpdateIDOL();
		break;
	case BossEnemy::MOVE:
		UpdateMOVE();
		break;
	case BossEnemy::SUMMON:
		UpdateSUMMON();
		break;
	case BossEnemy::ATTACK:
		UpdateATTACK();
		break;
	case BossEnemy::DAMAGE:
		UpdateDAMAGE();
		break;
	case BossEnemy::CRUSH:
		UpdateCRUSH();
		break;
	case BossEnemy::SPECIAL:
		UpdateSpecialATK();
		break;
	case BossEnemy::STOMP:
		UpdateSTOMP();
		break;
	default:
		reqBehavior_ = IDOL;
		break;
	}
	/*
	std::list<std::unique_ptr<SomeEnemy>>::iterator itr = enemies_.begin();
	for (; itr != enemies_.end(); ++itr)
	{
		itr->get()->Update();
	}*/

	// List 更新
	UpdateLists();

	if (isInvisible_)
	{
		invisibleFrame_--;
		if (invisibleFrame_ <= 0)
		{
			isInvisible_ = false;
		}
	}

	world_.UpdateMatrix();
	collider_->Update();

	HPBarUpdate();
}

void BossEnemy::DebagWindow()
{
#ifdef _DEBUG

	//model_->DebugParameter("boss");
	collider_->Debug("boss");

	ImGui::Begin("boss");

	ImGui::DragInt("HP", &HP_);

	ImGui::DragFloat3("position", &world_.translate_.x, 0.01f);

	ImGui::Text("invisible  : %d", invisibleFrame_);
	ImGui::Text("isInvisible: %s", isInvisible_ ? "TRUE" : "FALSE");
	ImGui::Text("moment     : %d", momentFrame_);
	//ImGui::Text("isActive   : %s", isActive_ ? "TRUE" : "FALSE");
	ImGui::Separator();
	switch (behavior_)
	{
	case BossEnemy::IDOL:
		ImGui::Text("IDOL");
		break;
	case BossEnemy::MOVE:
		ImGui::Text("MOVE");
		break;
	case BossEnemy::SUMMON:
		ImGui::Text("SUMMON");
		break;
	case BossEnemy::ATTACK:
		ImGui::Text("ATTACK");
		break;
	case BossEnemy::STOMP:
		ImGui::Text("STOMP");
		break;
	case BossEnemy::DAMAGE:
		ImGui::Text("DAMAGE");
		break;
	case BossEnemy::CRUSH:
		ImGui::Text("CRUSH");
		break;
	case BossEnemy::SPECIAL:
		ImGui::Text("SPECIAL");
		break;
	default:
		break;
	}
	ImGui::Separator();

	ImGui::Separator();
	if (ImGui::Button("Summon"))
	{
		reqBehavior_ = SUMMON;
	}

	if (ImGui::Button("Attack"))
	{
		reqBehavior_ = ATTACK;
	}
	if (ImGui::Button("Special"))
	{
		reqBehavior_ = SPECIAL;
	}
	if (ImGui::Button("Stomp"))
	{
		reqBehavior_ = STOMP;
	}

	std::list<std::unique_ptr<SomeEnemy>>::iterator itr = enemies_.begin();
	int num = 0;
	for (; itr != enemies_.end(); ++itr)
	{
		itr->get()->DebagWindow(num++);
	}

	ImGui::Text("specialATK  isATK : nextCount / %d : %d", specialATK.isSpecialATK_, specialATK.nextSpecialATKCount);

	ImGui::Text("Boss UI");
	ImGui::DragFloat3("ui pos", &uiWorld_.translate_.x);
	ImGui::DragFloat3("ui scale", &uiWorld_.scale_.x, 0.01f);





	ImGui::End();


#endif // _DEBUG

}

void BossEnemy::Draw()
{
	if (!isActive_)
	{
		return;
	}
	std::list<std::unique_ptr<SomeEnemy>>::iterator itr = enemies_.begin();
	for (; itr != enemies_.end(); ++itr)
	{
		itr->get()->Draw();
	}
	std::list<std::unique_ptr<AreaAttack>>::iterator itra = attacks_.begin();
	for (; itra != attacks_.end(); ++itra)
	{
		itra->get()->Draw();
	}
	GameObject::Draw(*camera_);
	collider_->Draw();


	hpBarBack_->Draw();
	hpBar_->Draw();
	hpBarFrame_->Draw();

	if (specialATK.isShot)
	{
		collider_->Draw();
		specialATK.ammo->Draw();
	}
}

bool BossEnemy::OnCollision(float damage, bool issPecial)
{
	// スペシャル攻撃中だった場合
	if (behavior_ == SPECIAL)
	{
		// 跳ね返しなら
		if (issPecial)
		{
			damage_ = damage;
			reqBehavior_ = DAMAGE;
			return true;
		}
		else
		{
			return false;
		}
	}
	//特殊攻撃使用時ではない
	else if (!isInvisible_)
	{
		damage_ = damage;
		reqBehavior_ = DAMAGE;
		return true;
	}
	return false;
}

void BossEnemy::SPATKOnCollison(const Vector3& direc)
{
	//座標を変更して移動方向変更
	/*Vector3 trans = specialATK.ammo->GetWorld().translate_;
	trans += direc;
	specialATK.ammo->SetTranslate(trans);*/
	specialATK.velocity_ = direc;
	specialATK.velocity_.y = 0;
	specialATK.velocity_.SetNormalize();
	specialATK.velocity_ *= specialATK.ammoSpd_;
	world_.UpdateMatrix();

	//プレイヤーが反射したという処理を消す
	specialATK.isHitPlayerWall = false;
}

void BossEnemy::SPATKOnColliExplo()
{
	//以下爆発
	ExploData data;
	data.world = specialATK.ammo->GetWorld();
	data.maxDeadCount = 60 * 1;
	data.minScale = 0;
	data.maxScale = specialATK.ammo->GetWorld().scale_.x * 2.0f;
	data.maxScaleCount = 30;
	data.mincolor = { 1,1,1,0.5f };
	data.minAlphaCount = 30;

	EfSphereExplosion::GetInstance()->AddEffectData(data);

	//カウントを最大まで進める
	int stateCount = specialATK.stateCount;
	specialATK.count[stateCount].count = specialATK.count[stateCount].maxCount;
}

void BossEnemy::SPATKReflectOnCollision(const Vector3& direc)
{
	//座標を変更して移動方向変更
	/*Vector3 trans = specialATK.ammo->GetWorld().translate_;
	trans+= direc;
	specialATK.ammo->SetTranslate(trans);*/
	specialATK.velocity_ = direc;
	specialATK.velocity_.y = 0;
	specialATK.velocity_.SetNormalize();
	specialATK.velocity_ *= specialATK.refrectSpd_;
	world_.UpdateMatrix();

	//プレイヤーが反射したという処理を入れる
	specialATK.isHitPlayerWall = true;
}

void BossEnemy::SeePlayer()
{
	//ボスからプレイヤーへのベクトル
	Vector3 seen = playerW_->GetMatWorldTranslate() - world_.GetMatWorldTranslate();
	seen.SetNormalize();
	seen.y = 0;

	//muki
	if (seen != Vector3(0, 0, 0))
	{
		seen *= -1;
		world_.rotate_.y = GetYRotate({ seen.x,seen.z });
	}

}

void BossEnemy::UpdateLists()
{
	// 雑魚敵
	for (std::list<std::unique_ptr<SomeEnemy>>::iterator itr = enemies_.begin();
		itr != enemies_.end();)
	{
		if (!itr->get()->GetIsActive())
		{
			itr = enemies_.erase(itr);
			continue;
		}
		itr->get()->Update();
		++itr;
	}
	// 範囲攻撃
	for (std::list<std::unique_ptr<AreaAttack>>::iterator itr = attacks_.begin();
		itr != attacks_.end();)
	{
		if (!itr->get()->GetIsActive())
		{
			itr = attacks_.erase(itr);
			continue;
		}
		itr->get()->Update();
		++itr;
	}
}

void BossEnemy::SummmonEnemy(bool spawn)
{
	if (!spawn && rand() % 3 == 0)
	{
		return;
	}
	SomeEnemy* data = new SomeEnemy;
	data->Initialize(*playerW_);
	data->SetBehavior(SomeEnemy::Move);

	int length = (int)moveLength_;
	int rnd1 = length / 2 - rand() % length;
	int rnd2 = length / 2 - rand() % length;
	data->SetPosition({ (float)rnd1,1.0f,(float)rnd2 });
	enemies_.emplace_back(data);
}

void BossEnemy::CreateAttack()
{
	AreaAttack* data = new AreaAttack();
	AreaAttack::Infomation info;
	info.mode_ = AreaAttack::ATTACKMODE::aSPOT;
	info.popPosition_ = playerW_->translate_;
	info.power_ = 1.0f;
	data->Initialize(info);
	attacks_.emplace_back(data);

	//エリア生成
	DZoneEData effect;
	effect.maxCount = 60;
	effect.tenmetu = 3;
	effect.isSphere = true;
	effect.pos.st = playerW_->GetMatWorldTranslate();
	effect.pos.ed = playerW_->GetMatWorldTranslate();

	effect.scale.st = { 10.0f,10.0f,10.0f };
	effect.scale.ed = { 10.0f,10.0f,10.0f };

	DZoneEffect::GetInstance()->AddData(effect);
}

void BossEnemy::HPBarUpdate()
{
	//生きているときに処理
	if (isActive_)
	{
		uiWorld_.UpdateMatrix();

		//最大HPと現HPの比率計算
		float hphiritu = (float)HP_ / (float)maxHP_;

		//テクスチャのスケールを計算
		float scaleX = hpBarScale.x * hphiritu;

		//スケール計算
		Vector3 nowScale = hpBarScale;
		nowScale.x = scaleX;

		//変更
		hpBar_->SetScale(nowScale);
	}
}

void BossEnemy::UpdateIDOL()
{
	momentFrame_--;
	if (momentFrame_ <= 0)
	{
#pragma region 必殺技関係
		if (goodGage_ >= maxGoodGage_)
		{
			specialATK.isSpecialATK_ = true;
		}
		else
		{
			specialATK.isSpecialATK_ = false;
		}

		//条件達成＆カウントmax
		if (specialATK.isSpecialATK_ && specialATK.nextSpecialATKCount >= specialATK.maxNextSpecialATKCount)
		{
			reqBehavior_ = SPECIAL;
			return;
		}
#pragma endregion

		int rnd = rand() % 5;
		// 移動
		if (rnd == 0)
		{
			rnd = rand() % (2 + moveCount_);
			if (rnd == 0)
			{
				reqBehavior_ = MOVE;
			}
			else
			{
				reqBehavior_ = IDOL;
				momentFrame_ -= 10;
			}
		}
		// 範囲攻撃生成
		else if (rnd == 1)
		{
			rnd = rand() % (2 + attackCount_);
			if (rnd == 0)
			{
				reqBehavior_ = ATTACK;
			}
			else
			{
				reqBehavior_ = IDOL;
				momentFrame_ -= 10;
			}
		}
		// 落下攻撃生成
		else if (rnd == 2)
		{
			rnd = rand() % (2 + stompCount_);
			if (rnd == 0)
			{
				reqBehavior_ = STOMP;
			}
			else
			{
				reqBehavior_ = IDOL;
				momentFrame_ -= 10;
			}
		}
		// 雑魚敵生成
		else //if (rnd == 1 || rnd == 2)
		{
			if (enemies_.size() < 10)
			{
				reqBehavior_ = SUMMON;
			}
			else
			{
				reqBehavior_ = MOVE;
				momentFrame_ -= 10;
			}
		}
	}
}

void BossEnemy::UpdateMOVE()
{
	momentFrame_--;
	if (momentFrame_ == cMOVEFRAME_ / 2)
	{
		world_.translate_.x = nextPosition_.x;
		world_.translate_.z = nextPosition_.y;
	}
	else if (momentFrame_ <= 0)
	{
		reqBehavior_ = IDOL;
	}
}

void BossEnemy::UpdateSUMMON()
{
	momentFrame_--;
	/*
	if (momentFrame_ == cSUMMONFRAME_ / 2)
	{
		SummmonEnemy();
		SummmonEnemy();
		SummmonEnemy();
	}*/
	if (momentFrame_ % 20 == 0)
	{
		SummmonEnemy(true);
	}
	if (momentFrame_ <= 0)
	{
		reqBehavior_ = IDOL;
		momentFrame_ += 20;
	}
}

void BossEnemy::UpdateATTACK()
{
	momentFrame_--;
	if (momentFrame_ % (cATTACKFRAME_ / cATTACKFREQUENCY_) == cATTACKFRAME_ % cATTACKFREQUENCY_)
	{
		CreateAttack();
		if (momentFrame_ % (cATTACKFRAME_ / cATTACKFREQUENCY_) == cATTACKFRAME_ % (cATTACKFREQUENCY_ * 2))
		{
			SummmonEnemy();
		}
	}
	if (momentFrame_ <= 0)
	{
		reqBehavior_ = IDOL;
		momentFrame_ += 80;
		// 仮で敵を出してみる
		// 多分残す
		SummmonEnemy();
		SummmonEnemy();
	}
}

void BossEnemy::UpdateDAMAGE()
{
	momentFrame_--;
	if (momentFrame_ <= 0)
	{
		SetAplta(1.0f);
		reqBehavior_ = IDOL;
	}
}

void BossEnemy::UpdateCRUSH()
{
	//world_.scale_ += {0.05f, 0.05f, 0.05f};
	invisibleFrame_ = 10;
	momentFrame_--;
	if (momentFrame_ % 20 == 0)
	{
		//以下爆発
		ExploData data;
		data.world = world_;
		data.world.translate_.x += rand() % 11 - 5;
		data.world.translate_.z += rand() % 11 - 5;
		data.maxDeadCount = 30;
		data.minScale = 0;
		data.maxScale = cCRUSHMINIMUM_;
		data.maxScaleCount = 30;
		data.mincolor = { 1,1,1,0.5f };
		data.minAlphaCount = 10;

		EfSphereExplosion::GetInstance()->AddEffectData(data);
	}
	if (momentFrame_ <= 0)
	{
		isActive_ = false;
	}
}

void BossEnemy::UpdateSTOMP()
{
	momentFrame_--;
	// 半分以上で
	if (cSTOMPFRAME_ / 2 <= (int32_t)momentFrame_)
	{
		// 10 フレームごと
		if (momentFrame_ % (cSTOMPFRAME_ / 10) == 0)
		{
			nextPosition_.x = playerW_->translate_.x;
			nextPosition_.y = playerW_->translate_.z;
			Vector3 pos = playerW_->translate_;
			//エリア生成
			DZoneEData effect;
			effect.maxCount = float(cSTOMPFALLING_);
			effect.tenmetu = 2;
			effect.isSphere = true;
			effect.pos.st = pos;
			effect.pos.ed = pos;

			effect.scale.st = { 5.0f,10.0f,5.0f };
			effect.scale.ed = { 5.0f,10.0f,5.0f };

			DZoneEffect::GetInstance()->AddData(effect);
		}
	}

	// 場所決定
	if (momentFrame_ == cSTOMPFRAME_ / 2 - (cSTOMPFRAME_ / 10))
	{
		//nextPosition_.x = playerW_->translate_.x;
		//nextPosition_.y = playerW_->translate_.z;
		Vector3 pos = { nextPosition_.x,playerW_->translate_.y,nextPosition_.y };
		//エリア生成
		DZoneEData effect;
		effect.maxCount = float(cSTOMPFRAME_ / 2);
		effect.tenmetu = 5;
		effect.isSphere = true;
		effect.pos.st = pos;
		effect.pos.ed = pos;

		effect.scale.st = { 10.0f,10.0f,10.0f };
		effect.scale.ed = { 10.0f,10.0f,10.0f };

		DZoneEffect::GetInstance()->AddData(effect);
		world_.translate_.x = nextPosition_.x;
		world_.translate_.z = nextPosition_.y;
		world_.translate_.y = 22.0f;
	}
	if (momentFrame_ <= 10)
	{
		world_.translate_.y -= 2.0f;
	}
	if (momentFrame_ <= 0)
	{
		//以下爆発
		ExploData data;
		data.world = world_;
		data.world.translate_.y = 2.0f;
		data.maxDeadCount = cSTOMPFALLING_;
		data.minScale = 0;
		data.maxScale = cCRUSHMINIMUM_;
		data.maxScaleCount = 10;
		data.mincolor = { 1,1,1,0.5f };
		data.minAlphaCount = 10;

		EfSphereExplosion::GetInstance()->AddEffectData(data);

		world_.translate_.y = 2.0f;
		reqBehavior_ = IDOL;
		momentFrame_ += 60;
	}
}

void BossEnemy::UpdateSpecialATK()
{
	//カウント状態取得
	int stateCount = specialATK.stateCount;

	//弾の更新
	if (specialATK.isShot)
	{
		Vector3 newpos = specialATK.ammo->GetWorld().translate_ + specialATK.velocity_;
		specialATK.ammo->SetTranslate(newpos);

		//muki
		if (specialATK.velocity_ != Vector3(0, 0, 0))
		{
			specialATK.ammo->SetRotateY(GetYRotate({ specialATK.velocity_.x,specialATK.velocity_.z }));
		}
		specialATK.ammo->Update();
		specialATK.collider->Update();
	}


	//一定カウントで次の処理へ
	if (specialATK.count[stateCount].count++ >= specialATK.count[stateCount].maxCount)
	{

		//戻る状態ではない場合次の状態へ
		if (stateCount != back)
		{
			//次の状態へ
			specialATK.stateCount++;
			//以降の処理初期化要に設定
			stateCount = specialATK.stateCount;

			specialATK.count[stateCount].count = 0;

			//弾があったら消して爆発処理
			if (specialATK.isShot == true)
			{
				specialATK.isShot = false;


			}


			//攻撃を打つとき
			if (stateCount == shotedWait)
			{
				specialATK.isShot = true;

				//体の向きと反対に撃つのでベクトルを計算
				Vector3 offset = { 0,0,1 };
				offset = TransformNormal(offset, world_.matWorld_);
				offset.SetNormalize();
				offset *= specialATK.ammoSpd_;
				specialATK.velocity_ = offset;
				//弾の発射処理
				specialATK.ammo->SetTranslate(world_.GetMatWorldTranslate());
				specialATK.ammo->SetScale(10);
				// 最初の更新
				specialATK.ammo->Update();
				specialATK.collider->Update();
			}

		}
		else
		{
			//戻る処理が終わった時の処理
			reqBehavior_ = IDOL;

		}
	}
}
