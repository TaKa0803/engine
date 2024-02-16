#pragma once
#include"InstancingGameObject/InstancingGameObject.h"
#include<list>

class BrokenBody :public InstancingGameObject {

public://シングルトンパターン
	static BrokenBody* GetInstance();

private://シングルトンパターン

	BrokenBody() = default;
	~BrokenBody() = default;
	BrokenBody(const BrokenBody& o) = delete;
	const BrokenBody& operator=(const BrokenBody& o) = delete;



public:

	void Initialize();

	void Update();

	void Draw();

	void EffectOccurred(const WorldTransform& world, int spawnNum);


	struct EffectData {
		WorldTransform world;
		Vector3 velocity_;
		Vector3 accce_ = { 0,-0.1f,0 };
		float BulletForce = 0.4f;
		const int maxDeadCount = 120;
		int deadCount = 0;
		bool isdead_ = false;

	};

	float upSPD_ = 1.0f;


	std::list<EffectData*>effectDatas_;

};