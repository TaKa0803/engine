#pragma once
#include"InstancingGameObject/InstancingGameObject.h"
#include"Vector3.h"

class PlaneCollider : public InstancingGameObject {

public:

	void Initialize(const std::string& tag, const WorldTransform& parent);

	void Update();

	void Draw();

	void Debug(const char* name);

private:
	std::string colliderTag_;

	bool isDraw_ = true;

	//向きのオフセット
	const Vector3 offset = { 0,0,1.0f };

	//面の向き
	Vector3 normal{};
};