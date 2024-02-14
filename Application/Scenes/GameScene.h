#pragma once
#include "IScene/IScene.h"

#include<iostream>
#include<list>

#include"Input/Input.h"
#include"Camera/Camera.h"
#include"Object/Objects.h"
#include"Sprite/Sprite.h"

#include"Effect/EffectExplosion.h"
#include"Effect/EffectSphereExplo/ESphereExplo.h"
#include"Effect/dZoneE/DZone.h"
#include"Object/Step/Step.h"

class GameScene : public IScene {

public:

	GameScene();

	~GameScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:
	//デバッグウィンドウ
	void DebugWindows();

	//カメラ更新
	void CameraUpdate();

	//コリジョン
	void CheckCollision();

	//シーン変更
	void SceneChange();

	/// <summary>
	/// UI更新
	/// </summary>
	void UIUpdate();

	//エフェクト発生処理
	void AddEffect(const WorldTransform& spawnW);

	//Goodゲージ追加
	void AddGoodGage(float num);

	//BADゲージ追加
	void AddBadGage();
private:
	//キー入力
	Input* input_ = nullptr;
	//カメラクラス
	std::unique_ptr<Camera> camera_;

	float xrotateNum = 0.05f;
	float yrotatenum = 0.02f;

	///以下ゲーム実装
	std::unique_ptr<Plane>plane_;

	std::unique_ptr<Player>player_;

	std::unique_ptr<Stage>stage_;

	std::unique_ptr<BossEnemy> boss_;

	std::unique_ptr<Skydome>skydome_;

#pragma region オーディエンス

	float stepLength_=90;

	std::unique_ptr<Step>steps_[5];
#pragma endregion


	//エフェクト
	EffectExplosion* EffectExp_;
	//エフェクト
	std::string eTag_ = "box";

	EfSphereExplosion* eSExplo_;

	DZoneEffect* dZoneEM_;

#pragma region ゲームルール関連

	//GOODゲージ最大数
	const float maxGoodGage_ = 10;

	//GOODゲージ現カウント
	float goodGage_ = 0;


	//BADゲージ最大数
	const int maxBadGage_ = 10;

	//BADゲージ現カウント
	int badGage_ = 0;


	//画像の名前
	enum SpriteName {
		Frame,
		Back,
		Good,
		Bad,
		kNumSprite
	};


	//ゲージのスプライト
	std::unique_ptr<Sprite>gageSprite_[SpriteName::kNumSprite];
	
	//スプライトのパス
	std::string gageSpritePath[SpriteName::kNumSprite] = {
		"resources/AppResource/UI/Gage_Frame.png",
		"resources/AppResource/UI/Gage_BACK.png",
		"resources/AppResource/UI/Gage_GOOD.png",
		"resources/AppResource/UI/Gage_BAD.png"
	};

	//ゲージのUIの全体の位置
	WorldTransform UIWorld_ ;

	//UI全体の座標
	Vector3 UIPosition_ = { 7.8f,71.0f,0 };
	//UIのサイズ
	Vector3 UIScale_ = { 0.7f,0.76f,1.0f };

	//各バーのサイズ
	Vector3 barScale_ = { 842,117,1 };
#pragma endregion

	//当たり判定処理するか否か
	bool isCollisionBoss_ = true;


	//シーンチェンジ用
	std::string white = "resources/SystemResources/white.png";
	std::unique_ptr<Sprite>sceneC_;
	float alpha = 0;
	bool isSceneChange = false;
	bool isPreScene = false;

	std::unique_ptr<Sprite>ui_;


#pragma region 音
	int bgm, bound, hit,damage;
#pragma endregion

};