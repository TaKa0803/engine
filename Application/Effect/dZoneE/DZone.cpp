#include "DZone.h"
#include"cassert"

DZoneEffect* DZoneEffect::GetInstance()
{
	static DZoneEffect ins;
	return &ins;
}

void DZoneEffect::Initialize()
{
	IMM_ = InstancingModelManager::GetInstance();
	//タグの存在チェック
	if (!IMM_->SerchTag(box) || !IMM_->SerchTag(sphere)) {
		//タグ名ミス
		assert(false);
	}

	datas_.clear();
}

void DZoneEffect::Update()
{

	for (auto& data : datas_)
	{
		if (data.count++ >= data.maxCount) {
			data.isEnd = true;
			continue;
		}


		//ｔを算出
		float t = data.count / data.maxCount;

		WorldTransform world;

		Vector3 pos = { data.pos.st * (1.0f - t) + data.pos.ed * t };
		Vector3 scale = { data.scale.st * (1.0f - t) + data.scale.ed * t };

		world.translate_ = pos;
		world.scale_ = scale;

		world.UpdateMatrix();

		//できたわーるぢを渡す
		data.world = world;

		//色関係
		//総合変化数
		float swapNum = data.tenmetu * 2.0f;
		//0回目の目標値取得
		float maxChangeC = data.maxCount / swapNum;
		//値Ｔ取得
		float colorT = data.count / (maxChangeC * (data.swapCount + 1));

		//偶数で増加処理
		if (data.swapCount % 2 == 0) {
			data.color.w = data.stColor.w * (1.0f - colorT) + data.edColor.w * colorT;
		}
		else {
			data.color.w = data.edColor.w * (1.0f - colorT) + data.stColor.w * colorT;
		}

		if (data.count >= (maxChangeC * (data.swapCount + 1))) {
			data.swapCount++;
		}
	}


	//削除処理
	datas_.remove_if([](DZoneEData& data) {
		if (data.isEnd) {
			return true;
		}
		else {
			return false;
		}
		});

}

void DZoneEffect::Draw()
{

	for (auto& data : datas_) {


		if (data.isSphere) {
			IMM_->SetData(sphere, data.world, data.color);
		}
		else {
			IMM_->SetData(box, data.world, data.color);
		}

	}

}

void DZoneEffect::AddData(DZoneEData& data)
{
	datas_.emplace_back(data);
}

