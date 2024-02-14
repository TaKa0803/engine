#pragma once

#include "IScene/IScene.h"
#include"Input/Input.h"
#include"Camera/Camera.h"
#include"SingleGameObjects/GameObject.h"

class CGScnene : public IScene {

public:

	CGScnene();
	~CGScnene();

	void Initialize() override;
	void Update() override;

	void Draw() override;

private:

	void Debug();

	//キー入力
	Input* input_ = nullptr;

	std::unique_ptr<Camera> camera_;

	std::unique_ptr<GameObject>object;

	int ball;
};