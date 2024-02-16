#pragma once

#include "IScene/IScene.h"
#include"Input/Input.h"
#include"Camera/Camera.h"
#include<iostream>
#include"SphereCollider/SphereCollider.h"
#include"ColliderPlane/PlaneCollider.h"
#include"ColliderOBB/OBBCollider.h"

#include"Effect/EffectExplosion.h"

class DebugScene : public IScene {

public:

	DebugScene();

	~DebugScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:

	void Debug();

private:
	//キー入力
	Input* input_ = nullptr;

	WorldTransform cWorld_;
	std::unique_ptr<Camera> camera_;

	
	WorldTransform sw1;
	WorldTransform sw2;
	WorldTransform obbw;


	std::unique_ptr<SphereCollider> sp1;
	std::unique_ptr<SphereCollider> sp2;

	std::unique_ptr<OBBCollider> obb;



	//エフェクト
	EffectExplosion* EffectExp_;

	//エフェクト
	std::string eTag_ = "box";
};