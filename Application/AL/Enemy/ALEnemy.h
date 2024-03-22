#pragma once

#include"SingleGameObjects/GameObject.h"
#include"SphereCollider/SphereCollider.h"
#include<iostream>
#include"AL/items.h"


class ALEnemy : public InstancingGameObject {

public:
	/// <summary>
	/// 
	/// </summary>
	void Initialize(const Vector3& position, const WorldTransform* playerWorld);

	/// <summary>
	/// 
	/// </summary>
	void Update();

	//行列のみ更新
	void UpdateMatrix();

	bool Collision(SphereCollider* collider);

	//osimodosi
	Vector3 OshiDashi(SphereCollider* collider);

	/// <summary>
	/// 
	/// </summary>
	void Draw();

	//死んだか否か
	bool GetDead() { return isDead_; }

	//すでに当たっているか否か
	bool isHit() { return isHit_; }

	void AddTranslate(const Vector3& translate) { world_.translate_ += translate; }

	SphereCollider* GetCollider() { return collider_.get(); }
private:
	//プレイヤーのワールド
	const WorldTransform* playerWorld_;

	std::unique_ptr<SphereCollider>collider_;

#pragma region 影
	std::unique_ptr<InstancingGameObject>shadow;
#pragma endregion

#pragma region モデル関係
	//モデルの数
	static const int modelNum_ = 5;

	//タグ軍
	std::string tags[modelNum_] = {
		"Head",
		"LArm",
		"RArm",
		"LFoot",
		"RFoot"
	};

	enum Parts {
		HEAD,
		LARM,
		RARM,
		LFOOT,
		RFOOT
	};

	WorldTransform mWorlds[modelNum_] = {};


	//歩くアニメーション
	ModelAnimeParts walkData_;

	//止まったモーション
	ModelAnimeParts stopData_;

	ModelAnimeParts nowRoop_;

	int roopCount_ = 0;

	enum MoveState {
		StopS,
		MoveS,
		NoneS
	};

	MoveState moveState_ = NoneS;
#pragma endregion



	//移動速度
	Vector3 velocity_{};

	//最大速度になるまでのF数
	float maxSPDFrame = 60.0f;

	//個体差用
	const float maxSPD_ = 0.5f;
	const float minSPD_ = 0.3f;

	//移動速度
	float moveSPD_ = 0.5f;


	//ugokanakunarukyori
	float stopRange_ = 10.0f;

	//探知距離
	float serchRange_ = 30.0f;


	enum State {
		Normal,
		Hit
	};

	State state_ = Normal;


	const Vector3 hitVelo = { 0,1.5f,0 };
	//加速度
	Vector3 acce = { 0,-0.1f,0 };

	//吹っ飛び量
	float hitSPD_ = 0.5f;


	bool isDead_ = false;

	int HP_ = 1;

	float tHeight = 0;

	//破壊音
	int breakSound_;

	bool isHit_ =false;
};