#pragma once

#include "SingleGameObjects/GameObject.h"
#include "SphereCollider/SphereCollider.h"
#include "Input/Input.h"

#include <optional>
#include <list>

#include "EchoBlast.h"

class Camera;

class Player :public GameObject
{
private:

	enum BEHAVIOR
	{
		IDOL,	// なにもしてない
		//MOVE,	// 移動
		ATTACK,	// 攻撃してる？
		MIRROR,	// 反射
		MOMENT,	// 後隙的な動けない時間

		_COUNT,	// カウント用
	};

private:

	using ATTACKMODE = EchoBlast::ATTACKMODE;

	Camera* camera_ = nullptr;

	// 中心からの距離
	float moveLength_ = 0.0f;

	// 今向いている方向
	Vector2 direction_ = { 0.0f, 1.0f };

	Input* input_ = nullptr;
	BEHAVIOR behavior_ = IDOL;
	std::optional<BEHAVIOR> reqBehavior_ = std::nullopt;

	// 攻撃の種類
	ATTACKMODE mode_ = ATTACKMODE::aSPOT;

	std::unique_ptr<SphereCollider> collider_;

	// 衝撃波
	std::list<std::unique_ptr<EchoBlast>> blasts_;

	uint32_t momentFrame_ = 20;

	bool isInvisible_ = false;
	int32_t invisibleFrame_ = 0;

	// 反射板終了フラグ
	bool isEndMirror_ = false;

public:
	Player();

	// 値の初期化
	void Initialize();

	void Update();

	void DebagWindow();

	void Draw(const Camera& viewp);

	auto& GetEchoBlasts() { return blasts_; }

	bool OnCollision();

	// 押し出しを反映
	void BackVector(const Vector3& vector) { world_.translate_ += vector; }

	SphereCollider* GetCollider() { return collider_.get(); }

	void SetCamera(Camera* camera) { camera_ = camera; }
	bool GetIsInvisible() const { return isInvisible_; }

private:

	void UpdateIDOL();
	void UpdateMove();

	void UpdateATTACK();

	void UpdateMIRROR();

	void CreateEcho(const EchoBlast::Infomation& info);

	int shotSound_;
};
