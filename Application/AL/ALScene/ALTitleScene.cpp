#include"ALTitleScene.h"
#include"TextureManager/TextureManager.h"
#include"AudioManager/AudioManager.h"
#include"Scenes/Scenes.h"

#include<imgui.h>

ALTitleScene::ALTitleScene() {
	input_ = Input::GetInstance();

	int texture = TextureManager::LoadTex("resources/AppResource/AL/Title.png");
	sprite_.reset(Sprite::Create(texture, { 320,180 }, { 320,180 }, { 1280,720 }));

	texture = TextureManager::LoadTex("resources/AppResource/AL/Press.png");
	pressSp_.reset(Sprite::Create(texture, { 160,90 }, { 160,90 }, { 100,70 }));
	pressSp_->SetPosition({ 570,500 });
	pressSp_->SetScale({ 170,100 });

	texture = TextureManager::LoadTex("resources/AppResource/AL/B.png");
	BButtonSp_.reset(Sprite::Create(texture, { 180,90 }, { 90,90 }, { 70,70 }));
	BButtonSp_->SetPosition({ 750,500 });

	texture = TextureManager::LoadTex(white);
	sceneC_.reset(Sprite::Create(texture, { 1,1 }, { 1,1 }, { 1280,720 }));
	sceneC_->SetMaterialDataColor({ 0,0,0,1 });

	titleSound_ = AudioManager::LoadSoundNum("title");
}

ALTitleScene::~ALTitleScene() {}

void ALTitleScene::Initialize() {
	sceneXhangeCount_ = maxSceneChangeCount_;
	isSceneChange_ = false;
	preSceneChange_ = false;
	sceneC_->SetColorAlpha(1);

	tenmetuCount = 0;
	isDrawB_ = true;


	AudioManager::GetInstance()->StopAllSounds();
	AudioManager::PlaySoundData(titleSound_,0.08f);
}

void ALTitleScene::Update() {


	Debug();

	if (isDrawB_) {
		
		if (tenmetuCount++ >= maxTenmetu) {
			isDrawB_ = false;
			tenmetuCount = 0;
			BButtonSp_->SetColorAlpha(0);
		}

	}
	else {
		if (tenmetuCount++ >= maxTenmetu) {
			isDrawB_ = true;
			tenmetuCount = 0;
			BButtonSp_->SetColorAlpha(1);
		}
	}

	SceneChange();
	
}

void ALTitleScene::Draw() {
	sprite_->Draw();

	pressSp_->Draw();

	BButtonSp_->Draw();

	sceneC_->Draw();
}

void ALTitleScene::Debug() {

	Vector3 pos = pressSp_->GetPosition();
	Vector3 scale = pressSp_->GetScale();

#ifdef _DEBUG
	ImGui::Begin("sprite");
	ImGui::DragFloat3("pos", &pos.x);
	ImGui::DragFloat3("scale", &scale.x);
	ImGui::End();
#endif // _DEBUG

	pressSp_->SetPosition(pos);
	pressSp_->SetScale(scale);

}

void ALTitleScene::SceneChange()
{
	if (input_->TriggerKey(DIK_SPACE)) {
		isSceneChange_ = true;
	}

	if (input_->IsControllerActive() && input_->IsTriggerButton(kButtonB)) {
		isSceneChange_ = true;
	}

	if (input_->TriggerKey(DIK_ESCAPE)) {
		leaveGame = true;
	}


	if (isSceneChange_) {

		float alpha = float(sceneXhangeCount_ / maxSceneChangeCount_);

		sceneC_->SetColorAlpha(alpha);

		if (sceneXhangeCount_++ >= maxSceneChangeCount_) {
			sceneC_->SetColorAlpha(1);
			sceneNo = ALGAME;
		}
	}
	else {

		if (!preSceneChange_) {

			float alpha = float(sceneXhangeCount_ / maxSceneChangeCount_);

			sceneC_->SetColorAlpha(alpha);

			if (sceneXhangeCount_-- <= 0) {
				sceneXhangeCount_ = 0;
				sceneC_->SetColorAlpha(0);
				preSceneChange_ = true;
			}

		}
	}
}
