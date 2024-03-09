#pragma once
#include"Vector3.h"
#include"InstancingGameObject/InstancingGameObject.h"

#include<list>



class EffectImpact : public InstancingGameObject {

public:

	void Initialize();

	void Update();

	void Draw();

	void Spawn(const WorldTransform playerW);
private:
	struct ImpactData
	{
		WorldTransform base;

		WorldTransform center;

		WorldTransform pos;

		Vector3 velo;

		float milit=60.0f;
	};


	std::list<ImpactData*>datas_;

	int spawnNum = 10;
};