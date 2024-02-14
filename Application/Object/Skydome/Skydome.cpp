#include "Skydome.h"

Skydome::Skydome()
{
	InstancingGameObject::Initialize("SkyDome");
	IMM_->SetEnableShader(tag_, false);
}

void Skydome::Initialize()
{
	world_.SetScale(500.0f);

}



void Skydome::Draw()
{
InstancingGameObject::Update();
InstancingGameObject::Draw();

}
