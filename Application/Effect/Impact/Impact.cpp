#include"Impact.h"

EffectImpact* EffectImpact::GetInstance()
{
	static EffectImpact instance;
	return &instance;
}

void EffectImpact::Initialize()
{
	InstancingGameObject::Initialize("Impact");
}

void EffectImpact::Update()
{
	//InstancingGameObject::Update();

	for (auto& data : datas_) {
		data.pos.translate_ += data.velo;
		data.pos.UpdateMatrix();
	}

}

void EffectImpact::Draw()
{

	//InstancingGameObject::Draw();
	for (auto& data : datas_) {
		IMM_->SetData(tag_, data.pos);
	}

}
