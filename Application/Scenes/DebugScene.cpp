#include "DebugScene.h"
#include"Scenes.h"
#include"InstancingModelManager/InstancingModelManager.h"
#include"RandomNum/RandomNum.h"

DebugScene::DebugScene()
{
	input_ = Input::GetInstance();

	camera_ = std::make_unique<Camera>();

	sp1 = std::make_unique<SphereCollider>();
	sp2 = std::make_unique<SphereCollider>();

	obb = std::make_unique<OBBCollider>();


	//エフェクト
	EffectExp_ = EffectExplosion::GetInstance();
}

DebugScene::~DebugScene() {}


void DebugScene::Initialize()
{

	cWorld_.Initialize();
	camera_->Initialize();
	camera_->SetTarget(&cWorld_);


	sp1->Initialize("sp",sw1);
	sp2->Initialize("sp",sw2);
	obb->Initialize("sp",obbw);

	
	sw1.Initialize();
	sw2.Initialize();
	obbw.Initialize();

	sw1.translate_ = { 5,0,0 };
	sw2.translate_ = { -5,0,0 };


	EffectExp_->Initialize();

}

void DebugScene::Update()
{
	Debug();

	camera_->Update();


	Vector3 move = input_->GetAllArrowKey();


	sw1.UpdateMatrix();
	sw2.UpdateMatrix();
	obbw.UpdateMatrix();

	sp1->Update();
	sp2->Update();
	obb->Update();


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


	Vector3 backV;
	if (obb->IsCollision(sp1.get(), backV)) {
		sw1.translate_ -= backV;
		sw1.UpdateMatrix();
		sp1->Update();
	}
	if (obb->IsCollision(sp2.get(), backV)) {
		sw2.translate_ -= backV;
		sw2.UpdateMatrix();
		sp2->Update();
	}

	if (sp1->IsCollision(sp2.get(), backV)) {
		sw1.translate_ += backV;
		sw1.UpdateMatrix();
		sp1->Update();
	}
}

void DebugScene::Draw()
{
	
	sp1->Draw();
	sp2->Draw();
	obb->Draw();


	EffectExp_->Draw();

	InstancingModelManager::GetInstance()->DrawAllModel(camera_->GetViewProjectionMatrix());
}

void DebugScene::Debug()
{
	camera_->DrawDebugWindow("camera");

	sw1.DrawDebug("s1");
	sw2.DrawDebug("s2");
	obbw.DrawDebug("obb");

}

