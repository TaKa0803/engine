#pragma once
#include"WorldTransform/WorldTransform.h"
#include"InstancingModelManager/InstancingModelManager.h"
#include"InstancingGameObject/InstancingGameObject.h"
#include<list>

struct ExploData
{
	WorldTransform world;//world
	int count=0;//カウント
	int maxDeadCount;//死亡までのカウント
	float minScale=0;//最小サイズ
	float maxScale=1;//最大サイズ
	int maxScaleCount;//最大サイズになるまでのカウント

	Vector4 mincolor = {1,1,1,1};
	Vector4 maxColor = { 1,1,1,0 };
	Vector4 color = { 1,1,1,1 };

	int minAlphaCount;//アルファ消え始めるまでのカウント

	bool isDead = false;
};

class EfSphereExplosion : public InstancingGameObject{

public://シングルトンパターン
	static EfSphereExplosion* GetInstance();

private://シングルトンパターン

	EfSphereExplosion() = default;
	~EfSphereExplosion() = default;
	EfSphereExplosion(const EfSphereExplosion& o) = delete;
	const EfSphereExplosion& operator=(const EfSphereExplosion& o) = delete;

public:


	void Initialize(const std::string& tag);

	//エフェクトデータを追加
	void AddEffectData(const ExploData& eData);

	void Update();

	void Draw();

private:
	

	//データ群
	std::list<ExploData>datas_;

	int sound_;
};