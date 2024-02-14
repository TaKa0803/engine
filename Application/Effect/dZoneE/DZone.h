#pragma once
#include"InstancingModelManager/InstancingModelManager.h"

#include<string>
#include<list>

struct stEdData
{
	Vector3 st;//開始地点
	Vector3 ed;//終了地点
};

struct  DZoneEData
{
	bool isEnd = false;			//終了したか
	float count = 0;			//カウント
	Vector4 color = { 1,1,1,1 };//色
	int swapCount = 0;
	const Vector4 stColor = { 1,1,1,0 };
	const Vector4 edColor = { 1,1,1,1 };
	WorldTransform world;

	//以下設定必要
	float maxCount;		//総アニメーション数
	float tenmetu;		//点滅数
	
	stEdData pos;		//座標動き
	stEdData scale;		//拡縮動き

	bool isSphere;		//円か否か
	
};


class DZoneEffect {

public://シングルトンパターン
	static DZoneEffect* GetInstance();

private://シングルトンパターン

	DZoneEffect() = default;
	~DZoneEffect() = default;
	DZoneEffect(const DZoneEffect& o) = delete;
	const DZoneEffect& operator=(const DZoneEffect& o) = delete;

public:

	void Initialize();

	void Update();

	void Draw();

	void AddData(DZoneEData& data);

private:
	InstancingModelManager* IMM_;


	std::string box = "dBox";
	std::string sphere = "dSphere";

	std::list<DZoneEData>datas_;
};
