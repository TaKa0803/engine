#include "CGScene.h"
#include"TextureManager/TextureManager.h"

CGScnene::CGScnene()
{

	input_ = Input::GetInstance();
	camera_ = std::make_unique<Camera>();

	object = std::make_unique<GameObject>();
	terrain = std::make_unique<GameObject>();

	ball = TextureManager::LoadTex("resources/SystemResources/monsterBall.png");
}

CGScnene::~CGScnene()
{
}

void CGScnene::Initialize()
{
	object->Initialize("sphere");
	terrain->Initialize("terrain");

	camera_->Initialize();
	camera_->SetTarget(&object->GetWorld());
}

void CGScnene::Update()
{

	Vector3 move = input_->GetAllArrowKey();
	move.y = input_->GetWASD().z;

	pointLightPos_ += move;

	object->Update();
	terrain->Update();
	camera_->Update();

	Debug();
}

void CGScnene::Draw()
{
	object->Draw(*camera_,pointLightPos_,ball);
	terrain->Draw(*camera_, pointLightPos_);

}

void CGScnene::Debug()
{
	object->Debug("object");
	terrain->Debug("terrain");
	camera_->DrawDebugWindow("camera");
}
