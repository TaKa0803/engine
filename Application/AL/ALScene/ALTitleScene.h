#pragma once
#include "IScene/IScene.h"
#include"Input/Input.h"
#include"Sprite/Sprite.h"

class ALTitleScene : public IScene {

public:

	ALTitleScene();

	~ALTitleScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:

	void Debug();

	//シーン変更処理
	void SceneChange();

private:
	//キー入力
	Input* input_ = nullptr;

	std::unique_ptr<Sprite> sprite_;

	std::unique_ptr<Sprite>pressSp_;

	std::unique_ptr<Sprite>BButtonSp_;


	//シーンチェンジ用
	std::string white = "resources/SystemResources/white.png";
	std::unique_ptr<Sprite>sceneC_;

	//シーン転換後の処置
	bool preSceneChange_ = false;

	//シーン転換処理をするか否か
	bool isSceneChange_ = false;

	const float maxSceneChangeCount_ = 60;

	float sceneXhangeCount_ = 0;


	//点滅
	const int maxTenmetu = 30;
	int tenmetuCount = 0;
	bool isDrawB_ = true;


	int titleSound_;

};