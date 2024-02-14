#include "TitleScene.h"
#include"TextureManager/TextureManager.h"
#include"AudioManager/AudioManager.h"
#include<imgui.h>
#include"Scenes.h"

TitleScene::TitleScene() {

	input_ = Input::GetInstance();

	/*int texture = TextureManager::LoadTex("resources/AppResource/Title/Title.png");
	sprite_.reset(Sprite::Create(texture,{320,180},{320,180},{1280,720}));

	texture = TextureManager::LoadTex("resources/AppResource/Title/Press.png");
	pressSp_.reset(Sprite::Create(texture, { 160,90 }, { 160,90 }, { 100,70 }));
	pressSp_->SetPosition({570,500});
	pressSp_->SetScale({170,100});

	texture = TextureManager::LoadTex("resources/AppResource/UI/B.png");
	BButtonSp_.reset(Sprite::Create(texture, { 180,90 }, { 90,90 }, { 70,70 }));
	BButtonSp_->SetPosition({ 750,500 });*/

	int tex = TextureManager::LoadTex("resources/AppResource/Title/Title.png");
	title_.reset(Sprite::Create(tex, { 850,500 }, { 850,500 }, { 1280, 720 }));

	tex = TextureManager::LoadTex(white);
	sceneC_.reset(Sprite::Create(tex, { 1,1 }, { 1,1 }, { 1280,720 }));
	sceneC_->SetMaterialDataColor({ 0,0,0,1 });

	tex = TextureManager::LoadTex(space);
	space_.reset(Sprite::Create(tex, { 1,1 }, { 1,1 }, { 300,100 },{640,600}));

	tex = TextureManager::LoadTex("resources/AppResource/UI/B.png");
	Bsprite_.reset(Sprite::Create(tex, { 180,90 }, { 90,90 }, { 100,100 }, { 640,650 }));

	bgm_ = AudioManager::LoadSoundNum("CLEAR");
}

TitleScene::~TitleScene() {
}

void TitleScene::Initialize() {
	sceneC_->SetColorAlpha(1.0f);
	alpha = 1;
	isSceneChange = false;
	isPreScene = false;

	AudioManager::GetInstance()->StopAllSounds();
	AudioManager::GetInstance()->PlaySoundData(bgm_, true);
}

void TitleScene::Update() {

	
	Debug();

	SceneCahnge();
}

void TitleScene::Draw() {

	title_->Draw();

	//space_->Draw();

	Bsprite_->Draw();

	sceneC_->Draw();
}

void TitleScene::Debug() {

#ifdef _DEBUG
	ImGui::Begin("TitleScene");
	ImGui::Text("Space or B to GameScene");
	ImGui::Text("ESC to leave game");
	ImGui::End();
#endif // _DEBUG


}

void TitleScene::SceneCahnge()
{

	if (!isPreScene) {
		alpha -= 1.0f / 30.0f;

		sceneC_->SetColorAlpha(alpha);
		if (alpha <= 0.0f) {
			alpha = 0.0f;
			isPreScene = true;
		}
	}
	else {
		if (input_->TriggerKey(DIK_SPACE)) {
			isSceneChange = true;
		}

		if (input_->IsControllerActive() && input_->IsTriggerButton(kButtonB)) {
			isSceneChange = true;
		}

		if (input_->TriggerKey(DIK_ESCAPE)) {
			leaveGame = true;
		}
	}
	


	if (isPreScene&&isSceneChange) {
		alpha += 1.0f / 60.0f;

		sceneC_->SetColorAlpha(alpha);
		if (alpha >= 1.0f) {
			sceneNo = GAME;
		}
	}
}

