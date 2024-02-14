#include "FailScene.h"
#include<imgui.h>
#include"Scenes/Scenes.h"
#include"TextureManager/TextureManager.h"
#include"AudioManager/AudioManager.h"

FailScene::FailScene()
{
	input_ = Input::GetInstance();
	int tex = TextureManager::LoadTex(white);
	sceneC_.reset(Sprite::Create(tex, { 1,1 }, { 1,1 }, { 1280,720 }));
	sceneC_->SetMaterialDataColor({ 0,0,0,1 });

	tex = TextureManager::LoadTex("resources/AppResource/GameOver_Failed.png");
	sprite_.reset(Sprite::Create(tex, { 850,500 }, { 850,500 }, { 1280,720 }));

	tex = TextureManager::LoadTex("resources/AppResource/UI/B.png");
	Bsprite_.reset(Sprite::Create(tex, { 180,90 }, { 90,90 }, { 100,100 }, { 640,650 }));

	bgm_ = AudioManager::LoadSoundNum("FAIL");
}

FailScene::~FailScene()
{
}

void FailScene::Initialize()
{
	sceneC_->SetColorAlpha(1.0f);
	alpha = 1;
	isSceneChange = false;
	isPreScene = false;

	AudioManager::GetInstance()->StopAllSounds();
	AudioManager::GetInstance()->PlaySoundData(bgm_,true);
}

void FailScene::Update()
{
	Debug();

	SceneCahnge();
}

void FailScene::Draw()
{

	sprite_->Draw();
	Bsprite_->Draw();
	sceneC_->Draw();
}

void FailScene::Debug()
{
#ifdef _DEBUG
	ImGui::Begin("TitleScene");
	ImGui::Text("Space or B to TitleScene");
	ImGui::Text("ESC to leave game");
	ImGui::End();
#endif // _DEBUG

}

void FailScene::SceneCahnge()
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

		if (isSceneChange) {
			alpha += 1.0f / 60.0f;

			sceneC_->SetColorAlpha(alpha);
			if (alpha >= 1.0f) {
				sceneNo = TITLE;
			}
		}
	}

	if (input_->TriggerKey(DIK_ESCAPE)) {
		leaveGame = true;
	}
}
