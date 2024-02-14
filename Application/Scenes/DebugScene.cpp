#include "DebugScene.h"
#include"Scenes.h"
#include"InstancingModelManager/InstancingModelManager.h"
#include"RandomNum/RandomNum.h"

DebugScene::DebugScene()
{
	input_ = Input::GetInstance();

	camera_ = std::make_unique<Camera>();

	/*pHead_ = std::make_unique<InstancingGameObject>();
	pLA_ = std::make_unique<InstancingGameObject>();
	pRA_ = std::make_unique<InstancingGameObject>();
	pLF_ = std::make_unique<InstancingGameObject>();
	pRF_ = std::make_unique<InstancingGameObject>();
	pHI_ = std::make_unique<InstancingGameObject>();

	pHead_->SetParent(&cWorld_);
	pLA_->SetParent(&pHead_->GetWorld());
	pRA_->SetParent(&pHead_->GetWorld());
	pLF_->SetParent(&pHead_->GetWorld());
	pRF_->SetParent(&pHead_->GetWorld());
	pHI_->SetParent(&pHead_->GetWorld());

	pLA_->SetTranslate({ -0.8f,0,0 });
	pRA_->SetTranslate({ 0.8f,0,0 });
	pLF_->SetTranslate({ -0.6f,-1.0f,0 });
	pRF_->SetTranslate({ 0.6f,-0.98f,0 });
	pHI_->SetTranslate({0,0.93f,0});*/




	//エフェクト
	EffectExp_ = EffectExplosion::GetInstance();
}

DebugScene::~DebugScene() {}


void DebugScene::Initialize()
{

	cWorld_.Initialize();
	camera_->Initialize();
	camera_->SetTarget(&cWorld_);

	/*pHead_->Initialize("pH");
	pLA_->Initialize("pLA");
	pRA_->Initialize("pRA");
	pLF_->Initialize("pLF");
	pRF_->Initialize("pRF");
	pHI_->Initialize("pHI");*/

	

	EffectExp_->Initialize();

}

void DebugScene::Update()
{
	Debug();

	camera_->Update();


	Vector3 move = input_->GetAllArrowKey();



	Vector3 backV;


	if (input_->TriggerKey(DIK_SPACE)) {

		EffectData newData;

		newData.tag = eTag_;

		for (int i = 0; i < 10; ++i) {

			moveData movedata;

			movedata.world = { 0,2,0 };
			movedata.world.scale_ = { 0.2f,0.2f ,0.2f };
			movedata.velo = {
				RandomNumber::Get(-1,1),
				RandomNumber::Get(-1,1),
				RandomNumber::Get(-1,1)
			};

			movedata.velo.SetNormalize();
			movedata.velo *= 1.0f;

			movedata.acce = { 0,-0.1f,0 };

			movedata.maxDeadCount = 60;

			newData.mData.push_back(movedata);

		}
		EffectExp_->AddEffectData(newData);
	}

	EffectExp_->Update();

	/*pHead_->Update();
	pLA_->Update();
	pRA_->Update();
	pLF_->Update();
	pRF_->Update();
	pHI_->Update();*/

	

}

void DebugScene::Draw()
{
	/*pHead_->Draw();
	pLA_->Draw();
	pRA_->Draw();
	pLF_->Draw();
	pRF_->Draw();
	pHI_->Draw();*/
	


	EffectExp_->Draw();

	InstancingModelManager::GetInstance()->DrawAllModel(camera_->GetViewProjectionMatrix());
}

void DebugScene::Debug()
{
	camera_->DrawDebugWindow("camera");

	/*pHead_->Debug("pH");
	pLA_->Debug("pLA");
	pRA_->Debug("pRA");
	pLF_->Debug("pLF");
	pRF_->Debug("pRF");
	pHI_->Debug("pHI");

	h_->Debug("h");
	B_->Debug("B");
	LA_->Debug("LA");
	RA_->Debug("RA");
	*/
}

