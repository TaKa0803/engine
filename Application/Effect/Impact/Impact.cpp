#include"Impact.h"
#include<numbers>

void EffectImpact::Initialize()
{
	InstancingGameObject::Initialize("Impact");
}

void EffectImpact::Update()
{
	//InstancingGameObject::Update();

	for (auto& data : datas_) {

		data->milit--;

		data->pos.translate_ += data->velo;

		data->base.UpdateMatrix();
		data->center.UpdateMatrix();
		data->pos.UpdateMatrix();
	}

	datas_.remove_if([](auto& data) {
		if (data->milit <= 0) {
			return true;
		}
		else {
			return false;
		}
	});


}

void EffectImpact::Draw()
{

	//InstancingGameObject::Draw();
	for (auto& data : datas_) {
		IMM_->SetData(tag_, data->pos);
	}

}

void EffectImpact::Spawn(const WorldTransform pos)
{
	//飛ぶ方向
	Vector3 offset = { 0,1,0 };

	float addRotate =( (float)std::numbers::pi * 2.0f)/10;

	//各回転後の座標を求める
	

	for (int i = 0; i < spawnNum; i++) {
		
		ImpactData* newData = new ImpactData();

		newData->base = pos;

		newData->center.parent_ = &newData->base;
		newData->center.translate_ = { 0,0,2 };
		newData->center.rotate_.z = addRotate * (float)i;
		newData->center.rotate_.x = -(float)std::numbers::pi*(2.0f / 5.0f);
		newData->pos.parent_ = &newData->center;
		newData->pos.translate_ = offset;

		newData->velo = { 0,1,0 };

		newData->velo.SetNormalize();
		newData->velo *= 3.0f;

		newData->milit = 10;

		datas_.push_back(newData);

	}
}
