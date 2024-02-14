#pragma once

#include <optional>

#include "InstancingGameObject/InstancingGameObject.h"
#include "ColliderOBB/OBBCollider.h"

class AreaAttack : public InstancingGameObject
{
public:
	//static const std::string kSPOTNAME_ = "PAmmo";
	//static const std::string kARCNAME_ = "SRS";

	enum ATTACKMODE
	{
		aSPOT,
		aARC,
	};

	struct Infomation
	{
		ATTACKMODE mode_;
		Vector3 popPosition_;
		float power_;
	};

private:

	enum BEHABIOR
	{
		SETUP,		// 準備
		OUTBREAK,		// 発生
		SUSTAIN,	// 持続
	};

private:
	Infomation data_;

	// 攻撃の中心地
	// 保存用
	WorldTransform colliderWorld_;
	// 当たり判定をたくさん取る
	//std::vector<std::unique_ptr<SphereCollider>> spheres_;
	std::unique_ptr<SphereCollider> collider_;

	// 生きているフレーム
	uint32_t momentFrame_ = 60;

	BEHABIOR behavior_ = SETUP;
	std::optional<BEHABIOR> reqBehavior_ = std::nullopt;

	bool isActive_ = false;

public:

	void Initialize(const Infomation& info);

	void Update();

	void Draw();

	void DebugWindow();

	bool GetIsActive()const { return isActive_; }

	SphereCollider* GetCollider() { return collider_.get(); }

	void OnCollision();

	void SetExplo();
};