#pragma once

#include "InstancingGameObject/InstancingGameObject.h"
#include "ColliderOBB/OBBCollider.h"

class EchoBlast : public InstancingGameObject
{
public:
	//static const std::string kSPOTNAME_ = "PAmmo";
	//static const std::string kARCNAME_ = "SRS";

	enum ATTACKMODE
	{
		aSPOT,
		aMIRROR,
	};

	struct Infomation
	{
		ATTACKMODE mode_;
		Vector3 popPosition_;
		Vector2	direction_;
		float power_;
	};

private:
	Infomation data_;

	// 攻撃の中心地
	// 保存用
	WorldTransform colliderWorld_;
	// 当たり判定をたくさん取る
	//std::vector<std::unique_ptr<SphereCollider>> spheres_;
	std::unique_ptr<OBBCollider> collider_;

	Vector3 direct3_;

	// 生きているフレーム
	uint32_t aliveFrame_ = 80;

	bool isActive_ = false;

public:

	void Initialize(const Infomation& info);

	void Update();

	void Draw();

	void DebugWindow();

	bool GetIsActive()const { return isActive_; }

	OBBCollider* GetCollider() { return collider_.get(); }

	void OnCollision();

	void SetInfomation(const Infomation& info);
	const Vector3& GetDirection() const { return direct3_; }

	// 弾くやつか
	bool GetIsSpot() const { return data_.mode_ == ATTACKMODE::aSPOT; }
};