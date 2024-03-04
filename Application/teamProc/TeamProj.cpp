#include "TeamProj.h"

TeamProj::TeamProj()
{
	player_ = std::make_unique<TeamPlayer>();

	camera_ = std::make_unique<Camera>();
}

TeamProj::~TeamProj()
{
	
}

void TeamProj::Initialize()
{
	player_->Initialize();

	camera_->Initialize();

	camera_->SetTarget(&player_->GetWorld());
}

void TeamProj::Update()
{
	Debug();
	

	

	player_->Update();

	camera_->Update();



}

void TeamProj::Draw()
{
	player_->Draw(*camera_);
}

void TeamProj::Debug()
{
	player_->Debug("player");

	camera_->DrawDebugWindow("camera");
}
