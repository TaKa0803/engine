#pragma once
#include "IScene/IScene.h"

#include"Input/Input.h"

#include"Sprite/Sprite.h"
#include<string>

class ClearScene :public IScene
{
public:

	ClearScene();

	~ClearScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:

	//デバッグ
	void Debug();

	//シーンチェンジ
	void SceneCahnge();
private:
	//キー入力
	Input* input_ = nullptr;

	std::string space = "resources/AppResource/GameOver_Clear.png";
	std::unique_ptr<Sprite>space_;

	//シーンチェンジ用
	std::string white = "resources/SystemResources/white.png";
	std::unique_ptr<Sprite>sceneC_;
	float alpha = 0;
	bool isSceneChange = false;
	bool isPreScene = false;

	std::unique_ptr<Sprite>Bsprite_;

	int bgm_;
};

