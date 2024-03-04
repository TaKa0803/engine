#include "teamP.h"

void TeamPlayer::Initialize()
{
	GameObject::Initialize("ALPlayer");
}

void TeamPlayer::Update()
{

	world_.translate_ += moveVelo;


	GameObject::Update();
}

void TeamPlayer::Draw(const Camera& camera)
{
	GameObject::Draw(camera);
}
