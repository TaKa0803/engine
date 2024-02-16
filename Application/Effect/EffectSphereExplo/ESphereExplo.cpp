#include "ESphereExplo.h"
#include"AudioManager/AudioManager.h"

EfSphereExplosion* EfSphereExplosion::GetInstance()
{
	static EfSphereExplosion instance;
	return &instance;
}

void EfSphereExplosion::Initialize(const std::string& tag)
{
	InstancingGameObject::Initialize(tag);

	IMM_->SetEnableShader(tag_, false);
	IMM_->SetBlendMode(tag_, BlendMode::kAdd);

	datas_.clear();

	sound_ = AudioManager::LoadSoundNum("maxExplo");
}

void EfSphereExplosion::AddEffectData(const ExploData& eData)
{
	datas_.emplace_back(eData);

	AudioManager::PlaySoundData(sound_,0.5f);
}

void EfSphereExplosion::Update()
{

	for (auto& data : datas_) {

		if (data.count++ >= data.maxDeadCount) {
			data.isDead = true;
			continue;
		}

		//data.world.scale_ = { data.maxScale,data.maxScale,data.maxScale };
		//data.color.w = 0.5f;
		
		//サイズ関係
		float t = float(data.count) /float( data.maxScaleCount);
		if (t >= 1.0f) {
			t = 1.0f;
		}
		float scale = data.minScale * (1.0f - t) + data.maxScale * t;
		data.world.scale_ = { scale,scale ,scale };

		
		//色関係
		if (data.count >= data.minAlphaCount) {
			float colort = float((data.count-data.minAlphaCount) / float(data.maxDeadCount - data.minAlphaCount));
			data.color = Esing(data.mincolor, data.maxColor, colort);
		}
		

		data.world.UpdateMatrix();
	}

	//死亡チェック
	datas_.remove_if([](ExploData& data) {
		if (data.isDead) {
			return true;
		}
		else {
			return false;
		}
		});

}

void EfSphereExplosion::Draw()
{
	for (auto& data : datas_) {
		IMM_->SetData(tag_, data.world, data.color);
	}
}

