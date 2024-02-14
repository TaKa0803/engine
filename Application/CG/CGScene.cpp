#include "CGScene.h"
#include"TextureManager/TextureManager.h"

CGScnene::CGScnene()
{

	input_ = Input::GetInstance();
	camera_ = std::make_unique<Camera>();

	object = std::make_unique<GameObject>();

	ball = TextureManager::LoadTex("resources/SystemResources/monsterBall.png");
}

CGScnene::~CGScnene()
{
}

void CGScnene::Initialize()
{
	object->Initialize("sphere");
	camera_->Initialize();
	camera_->SetTarget(&object->GetWorld());
}

void CGScnene::Update()
{
	object->Update();
	camera_->Update();

	Debug();
}

void CGScnene::Draw()
{
	object->Draw(*camera_,ball);
}

void CGScnene::Debug()
{
	object->Debug("object");
	camera_->DrawDebugWindow("camera");
}
