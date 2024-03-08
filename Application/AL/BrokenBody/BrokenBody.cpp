#include"BrokenBody.h"
#include"RandomNum/RandomNum.h"

BrokenBody* BrokenBody::GetInstance() {
	static BrokenBody instance;
	return&instance;
}

void BrokenBody::Initialize() {
	InstancingGameObject::Initialize("EBox");
	effectDatas_.clear();
}

void BrokenBody::Update() {




	for (EffectData* eData : effectDatas_) {

		if (eData->deadCount++ >= eData->maxDeadCount) {
			eData->isdead_ = true;
		}
		else {

			eData->velocity_ += eData->accce_;
			eData->world.translate_ += eData->velocity_;

			if (eData->world.translate_.y <= 0) {
				eData->world.translate_.y = 0;
				eData->velocity_.y *= -1;
				eData->velocity_ *= eData->BulletForce;
			}


		}
	}


	effectDatas_.remove_if([](const EffectData* edata) {
		if (edata->isdead_) {
			return true;
		}
		return false;
		});
}

void BrokenBody::Draw() {

	for (auto& eData : effectDatas_) {
		eData->world.UpdateMatrix();

		IMM_->SetData(tag_, eData->world,color_);
	}

}

void BrokenBody::EffectOccurred(const WorldTransform& world, int spawnNum) {

	for (int count = 0; count < spawnNum; ++count) {

		EffectData* edata = new EffectData();

		//ワールド
		edata->world = world;
		edata->velocity_ = {
			RandomNumber::Get(-upSPD_,upSPD_),
			upSPD_,
			RandomNumber::Get(-upSPD_,upSPD_)
		};

		effectDatas_.push_back(edata);
	}


}