#pragma once

#include "IScene/IScene.h"

#include<iostream>
#include<list>

#include"Input/Input.h"
#include"Camera/Camera.h"

#include"AL/Player/ALPlayer.h"
#include"AL/Plane/Plane.h"
#include"AL/Enemy/ALEnemy.h"
#include"AL/EnemyPopManager/EnemyPopManager.h"
#include"Sprite/Sprite.h"
#include"AL/BrokenBody/BrokenBody.h"


class ALGameScene : public IScene {

public:

	ALGameScene();

	~ALGameScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:
	void DebugWindows();

	void Collision();

	void SceneChange();

	void UIUpdateDraw();

	void ClearUIUpdate();

	void LimitUI();

	void ShakeStart(int count);

	void CameraShake();
private:
	//キー入力
	Input* input_ = nullptr;
	//カメラクラス
	std::unique_ptr<Camera> camera_;

	float xrotateNum = 0.05f;
	float yrotatenum = 0.02f;

	///以下ゲーム実装
	std::unique_ptr<ALPlayer>player_;

	std::unique_ptr<Plane>plane_;

	std::list<std::unique_ptr<ALEnemy>>enemies_;

	std::unique_ptr<EnemyPopManager>enemyPopManager_;

	int limitMinute = 0;
	const int maxmilitMinute = 60 * 60;

	std::unique_ptr<InstancingGameObject>coin_;

	enum Scene {
		Game,
		Clear,
	};

	Scene scene_ = Game;

	std::unique_ptr<Sprite>skillSp_;

	std::unique_ptr<Sprite>BButton_;

	std::unique_ptr<Sprite>punchSp_;

	std::unique_ptr<Sprite>kickSp_;

	std::unique_ptr<Sprite>ultSp_;

	Vector3 waitATKpos = { 1000,622,0 };
	Vector3 waitATKscale = { 90,90 };

	Vector3 ATKpos = { 1085,560 };
	Vector3 ATKscale = { 200,200 };

	enum ButtonState {
		Wait,
		Punch,
		Kick,
		Ult
	};

	ButtonState BState_ = Wait;


	std::unique_ptr<Sprite>num1_;
	std::unique_ptr<Sprite>num10_;


	std::unique_ptr<Sprite>num100_;

	std::unique_ptr<Sprite>backScreen_;
	std::unique_ptr<Sprite>resultText_;

	float alphaNum_ = 0;

	const float addAlphaNum_ = 120.0f;

	const float screenmaxAlphaNum_ = 0.7f;


	//1フレームごとに計算する敵の数
	const float frameCountEnemy_ = 1.0f;

	float nowFrameCountEnemy_ = 0;

	bool serchComplete_ = false;


	BrokenBody* brokenBody_;

	//シーンチェンジ用
	std::string white = "resources/SystemResources/white.png";
	std::unique_ptr<Sprite>sceneC_;

	//シーン転換後の処置
	bool preSceneChange_ = false;

	//シーン転換処理をするか否か
	bool isSceneChange_ = false;

	const float maxSceneChangeCount_ = 60;

	float sceneXhangeCount_ = 0;

	int bgmGame_;

	int bgmClear_;

	bool isShake_ = false;
	int cameraShakeCount_=0;
	Vector2 shakenum = {0,0};
	Vector3 tempP_;
};