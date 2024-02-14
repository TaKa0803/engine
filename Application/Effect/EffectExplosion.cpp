#include "EffectExplosion.h"

EffectExplosion* EffectExplosion::GetInstance()
{
	static EffectExplosion instance;
	return &instance;
}

void EffectExplosion::Initialize()
{
	IMM_ = InstancingModelManager::GetInstance();

	//現存分削除
	datas_.clear();
}

void EffectExplosion::AddEffectData(const EffectData& eData)
{
	datas_.emplace_back(eData);
}

void EffectExplosion::Update()
{

	//各データの動きのデータの処理
	for (auto& data : datas_) {

		//各パーツ更新
		for (auto& pdata : data.mData) {
			//移動量更新
			pdata.world.translate_ += pdata.velo;
			//加速度処理
			pdata.velo += pdata.acce;

			//0以下で停止
			if (pdata.world.translate_.y < 0) {
				pdata.world.translate_.y = 0;
				pdata.velo.SetZero();
			}

			//行列更新
			pdata.world.UpdateMatrix();

			//死亡チェック
			if (pdata.deadCount++>=pdata.maxDeadCount) {
				pdata.isDead = true;
			}
		}

		//死亡チェック
		data.mData.remove_if([](auto& data) {
			if (data.isDead) {
				return true;
			}
			else {
				return false;
			}
		});
	}


	//条件を満たしたデータの削除
	datas_.remove_if([](auto& data) {
		if (data.mData.size() == 0) {
			return true;
		}
		else {
			return false;
		}

	});

}

void EffectExplosion::Draw()
{

	for (auto& data : datas_) {

		//各ワールドデータ
		for (auto& worldData : data.mData) {
			//ワールド取り出し
			WorldTransform world = worldData.world;

			world.UpdateMatrix();

			//モデルにワールド送信
			IMM_->SetData(data.tag, world);
		}

	}

}
