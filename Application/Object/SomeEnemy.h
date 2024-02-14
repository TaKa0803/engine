#pragma once

#include "InstancingGameObject/InstancingGameObject.h"
#include "SphereCollider/SphereCollider.h"

#include <optional>
#include<iostream>

class SomeEnemy :public InstancingGameObject
{
public:

	//敵のタイプ
	enum SomeType {
		Move,
		Explo
	};

	enum BEHAVIOR
	{
		IDOL,	// なにもしてない
		MOVE,	// 移動
		BURST,	// 弾かれ中
		DESTROY,// 爆発
		ATK,	// 攻撃
		_COUNT,	// カウント用
	};

private:

	int32_t cBURSTFRAME_ = 300;
	int32_t cALIVEFRAME_ = 600;
	// 攻撃力
	int32_t cATTACKPOWER_ = 2;
	// 倍率
	float cATTACKFACTOR_ = 2.0f;

	// 中心からの距離
	float moveLength_ = 0.0f;

	// 今向いている方向
	Vector3 direct3_ = { 0.0f,0.0f, 0.0f };

	BEHAVIOR behavior_ = IDOL;
	std::optional<BEHAVIOR> reqBehavior_ = std::nullopt;

	//敵のタイプ
	SomeType eType_=Move;

	std::unique_ptr<SphereCollider> collider_;

	// 爆発までのフレーム
	uint32_t momentFrame_ = cBURSTFRAME_;

	// 処理するか
	bool isActive_ = false;

	// 倍率を掛けるか
	bool isFactor_ = false;

	// 既にボスに当たっているか
	bool isHit_ = false;

	//飛ぶ速度
	float burstSpd_ = 2.0f;

#pragma region 移動関係
	//プレイヤーのworldデータ
	const WorldTransform* playerW_ = nullptr;

	//移動速度
	float spd_ = 0.1f;

	//プレイヤーとの最短距離
	float nearPRadius_ = 5.0f;
#pragma endregion

#pragma region 攻撃関係
	//動きの各状態
	enum ATKState {
		wait,	//構え
		atk,	//攻撃
		stop,	//硬直
		back,	//戻る
		kCountOfATKState
	};
	
	//攻撃の各動きのカウント
	struct ATKCount
	{
		bool initialize=false;
		int maxCount = 0;
		int count = 0;
	};	
	ATKCount atkCount_[kCountOfATKState];

	//攻撃状態カウント
	int ATKStateCount_ = 0;

	//攻撃方向
	Vector3 atkDirection_{};

	//攻撃移動速度
	float atkSpd_ = 0.1f;
#pragma endregion


public:
	// 値の初期化
	void Initialize(const WorldTransform&world);

	void Update();

	void DebagWindow(int num);

	void Draw();

	SphereCollider* GetCollider() { return collider_.get(); }

	// ボスとぶつかった時
	/*bool OnCollision()
	{
		if (isHit_) { return false; }
		reqBehavior_ = DESTROY;
		isHit_ = true;
		return true;
	}*/
	// 壁、攻撃の時
	void OnCollision(const Vector3& direction);
	// 敵同士の衝突用
	void OnEnemy(const Vector3& direction);

	bool GetIsActive() const { return isActive_; }
	bool GetCanEnemy() const { return behavior_ == IDOL || behavior_ == MOVE; }
	bool GetIsBurst() const { return behavior_ == BURST; }
	bool GetIsDestroy() const { return behavior_ == DESTROY; }
	float GetAttackPower() const { return cATTACKPOWER_ * (isFactor_ ? cATTACKFACTOR_ : 1.0f); }
	const Vector3& GetDirection() const { return direct3_; }

	void SetBehavior(SomeType b) { eType_ = b; }
	void SetPosition(const Vector3& pos) { world_.translate_ = pos; }
private:
	
	void MoveEnemyUpdate();

	void ExpEnemyUpdate();

	void IDOLUpdate();

	//移動処理更新
	void MoveToPlayer();

	//攻撃処理更新
	void ATKToPlayerUpdate();

	//爆破処理設置
	void SetExplo();
};
