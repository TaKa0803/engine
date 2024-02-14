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

	/*std::unique_ptr<InstancingGameObject>pHead_;
	std::unique_ptr<InstancingGameObject>pLA_;
	std::unique_ptr<InstancingGameObject>pRA_;
	std::unique_ptr<InstancingGameObject>pLF_;
	std::unique_ptr<InstancingGameObject>pRF_;
	std::unique_ptr<InstancingGameObject>pHI_;*/


	//エフェクト
	EffectExplosion* EffectExp_;

	//エフェクト
	std::string eTag_ = "box";
};