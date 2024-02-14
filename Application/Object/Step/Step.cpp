#include"Step.h"

Step::Step()
{
	InstancingGameObject::Initialize("step");
	world_.scale_ = { 9.1f,2,3 };

	for (int i = 0; i < 3; i++) {
		for (int h = 0; h < 10; h++) {
			audience_[i][h] = std::make_unique<Audience>();
		}
	}
}

void Step::Initialize()
{

}

void Step::Update()
{
	InstancingGameObject::Update();
	for (int i = 0; i < 3; i++) {
		for (int h = 0; h < 10; h++) {
			audience_[i][h]->SetParent(&world_);
			audience_[i][h]->SetTranslate(Vector3((float)h * 0.5f - 5*0.5f, (float)i * 1 - 1 + 2.6f, (float)i * 2 - 2));
			audience_[i][h]->SetScale(Vector3(1.0f / 9.1f, 1.0f / 2.0f, 1 / 3.0f));
			audience_[i][h]->SetRotate(Vector3(0, 3.14f, 0));
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int h = 0; h < 10; h++) {
			audience_[i][h]->Update();
		}
	}

}

void Step::Draw()
{

	for (int i = 0; i < 3; i++) {
		for (int h = 0; h < 10; h++) {
			audience_[i][h]->Draw();
		}
	}
	InstancingGameObject::Draw();
}

void Step::Debug()
{
	for (int i = 0; i < 3; i++) {
		for (int h = 0; h < 10; h++) {
			audience_[i][h]->Debug("audience");
		}
	}
	world_.DrawDebug("step");
}
