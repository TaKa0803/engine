#pragma once
#include "IScene/IScene.h"
#include"Input/Input.h"
#include"Sprite/Sprite.h"

class TitleScene : public IScene {

public:

	TitleScene();

	~TitleScene();

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
	Input* input_=nullptr;

	std::unique_ptr<Sprite>title_;


	std::unique_ptr<Sprite>Bsprite_;

	//シーンチェンジ用
	std::string white = "resources/SystemResources/white.png";
	std::unique_ptr<Sprite>sceneC_;
	float alpha = 0;
	bool isSceneChange = false;
	bool isPreScene = false;

	std::string space = "resources/AppResource/space.png";
	std::unique_ptr<Sprite>space_;

	int bgm_;
};

