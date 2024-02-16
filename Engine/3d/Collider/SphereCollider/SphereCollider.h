#pragma once
#include"InstancingGameObject/InstancingGameObject.h"


class OBBCollider;


class SphereCollider : InstancingGameObject {

public:
	//
	SphereCollider();

	void Initialize(const std::string& tag,const WorldTransform& parent);

	void Initialize(const std::string& tag);


	void Update();

	void Draw();

	/// <summary>
	/// 当たり判定チェック
	/// </summary>
	/// <param name="sphere">元となるコライダー</param>
	/// <param name="backVec">押し戻しベクトル</param>
	/// <returns>押し戻しベクトルに値を渡し成否チェック</returns>
	bool IsCollision(const SphereCollider* sphere, Vector3& backVec);

	/// <summary>
	/// 当たり判定チェック
	/// </summary>
	/// <param name="obb">元となるコライダー</param>
	/// <param name="backVec">押し出しベクトル</param>
	/// <returns>引数の押し出しベクトルに値を渡して成否チェック</returns>
	bool IsCollision(OBBCollider& obb, Vector3& backVec, float divisionVolume);

	//デバッグImGui
	void Debug(const char* name);

	//透明度設定
	void SetAlpha(const float alpha) { alpha_ = alpha; }

	//半径設定
	void SetRadius(float wide) { radius_ = wide; world_.scale_ = { radius_,radius_,radius_ }; }

	//座標設定
	void SetTranslate(const Vector3& pos) { world_.translate_ = pos; world_.UpdateMatrix(); }

	//フラグに併せて色変更
	void SetColor(bool hit);

	//当たった時に行列更新するといい
	void UpdateMatrix();
public:

	//半径取得
	float GetRadius() { return radius_; }

	//ローカル平行移動取得
	Vector3 GetTranslate() { return world_.translate_; }

	//コライダータグ取得
	std::string GetTag()const { return colliderTag_; }

	//ワールド取得
	WorldTransform GetWorld()const { return world_; }

	
private:

	//半径
	float radius_=1.0f;
	//コライダータグ
	std::string colliderTag_;

	//描画するか否か(すべて
	static bool isDraw_;

	//過去の状態
	WorldTransform preWorld_;

	//透明度
	float alpha_ = 0.5f;

	//通常色
	Vector4 normalColor = { 1,1,1,alpha_ };

	//ヒット時色
	Vector4 hitColor = { 1,0,0,alpha_ };


};