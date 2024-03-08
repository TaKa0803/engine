#pragma once

#include"InstancingModelManager/InstancingModelManager.h"
#include"WorldTransform/WorldTransform.h"
#include<vector>

class InstancingGameObject {

protected:
	//モデルのタグ
	std::string tag_;

	//ワールド
	WorldTransform world_{};

	//インスタンシングモデルマネージャー
	InstancingModelManager* IMM_=nullptr;

	//色
	Vector4 color_ = { 1,1,1,1 };
public:
	//ワールド取得
	const WorldTransform& GetWorld()const { return world_; }

	void SetTranslate(const Vector3& trans) { world_.translate_ = trans; }

	void SetScale(float scale) { world_.scale_ = { scale,scale ,scale }; }

	void SetScale(const Vector3& scale) { world_.scale_ = scale; }


	void SetRotate(const Vector3& rotate) { world_.rotate_ = rotate; }

	void SetRotateY(const float rotate) { world_.rotate_.y = rotate; }

	void SetParent(const WorldTransform* parent) { world_.parent_ = parent; }

	void SetColor(const Vector4& color) { color_ = color; }

	//開放処理
	virtual ~InstancingGameObject();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="tag">モデルタグ</param>
	virtual void Initialize(const std::string& tag);

	/// <summary>
	/// 行列とワールド追加
	/// </summary>
	virtual void Update();

	virtual void Draw();

	virtual void Debug(const char*name);
};

