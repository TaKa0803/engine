#pragma once
#include"Vector3.h"
#include"InstancingGameObject/InstancingGameObject.h"

#include<vector>


struct ImpactData
{
	WorldTransform pos;

	Vector3 velo;
};

class EffectImpact : public InstancingGameObject {

public://シングルトンパターン
	static EffectImpact* GetInstance();

private://シングルトンパターン

	EffectImpact() = default;
	~EffectImpact() = default;
	EffectImpact(const EffectImpact& o) = delete;
	const EffectImpact& operator=(const EffectImpact& o) = delete;

public:

	void Initialize();

	void Update();

	void Draw();
private:
	std::vector<ImpactData>datas_;

};