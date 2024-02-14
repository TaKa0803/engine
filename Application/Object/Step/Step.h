#pragma once
#include"InstancingGameObject/InstancingGameObject.h"
#include"Object/Audience/audience.h"

class Step : public InstancingGameObject {

public:

	Step();

	void Initialize();

	void Update();

	void Draw();

	void Debug();


	void SetTranslate(const Vector3& translate) { world_.translate_ = translate; }

	void SetRotate(float y) { world_.rotate_.y = y; }

private:

	std::unique_ptr<Audience>audience_[3][10];
};
