#pragma once
#include"InstancingGameObject/InstancingGameObject.h"
#include"IsCollisions.h"

#include"Quaternion.h"

class SphereCollider;

class OBBCollider : public InstancingGameObject {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="tag">コライダーのタグ</param>
	/// <param name="parent">親のワールド</param>
	void Initialize(const std::string& tag, const WorldTransform& parent);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="tag">コライダーのタグ</param>
	void Initialize(const std::string& tag);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画（Releaseでは描画されない
	/// </summary>
	void Draw();

	/// <summary>
	/// デバッグImGui表示
	/// </summary>
	/// <param name="name">window名</param>
	void Debug(const char* name);

	/// <summary>
	/// 行列のみ更新(押し込み処理後に呼び出し
	/// </summary>
	void UpdateMatrix();

	/// <summary>
	/// Sphereコライダーとの判定
	/// </summary>
	/// <param name="collider">コライダーのポインタ</param>
	/// <param name="backVec">押し出し量</param>
	/// <returns>押し出し量を渡し、当たったか否か判定</returns>
	bool IsCollision(SphereCollider*collider,Vector3&backVec);

	//線との当たり判定
	bool IsCollision(const Segment& seg);

	/// <summary>
	/// 色の設定
	/// </summary>
	/// <param name="color"></param>
	void SetColor(const Vector4& color) { normalColor=color; }

	/// <summary>
	/// アルファ地の設定
	/// </summary>
	/// <param name="alpha"></param>
	void SetAlpha(const float alpha) { normalColor.w = alpha; }

	/// <summary>
	/// 正否に合わせた色に変更
	/// </summary>
	/// <param name="hit"></param>
	void SetColor(bool hit);

	/// <summary>
	/// サイズ変更
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(const Vector3& scale) { world_.scale_ = scale; }

	/// <summary>
	/// サイズ変更（等倍
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(const float scale) { world_.scale_ = { scale,scale,scale }; }

public:

	/// <summary>
	/// OBBのworldmatrix取得
	/// </summary>
	/// <returns></returns>
	const Matrix4x4& GetWorldM()const { return OBBM_; }

	/// <summary>
	/// worldmatrixの逆行列取得
	/// </summary>
	/// <returns></returns>
	const Matrix4x4& GetInverseWorldM()const { return inverseM_; }
private:
	//コライダータグ
	std::string colliderTag_;

	//すべての描画フラグ
	static bool isDraw_;

	Quaternion rotation_ = {};

	//透明度
	float alpha_ = 0.5f;

	//通常時色
	Vector4 normalColor = { 1,1,1,alpha_ };
	//ヒット時色
	Vector4 hitColor = { 1,0,0,alpha_ };

	

	struct OBB {
		Vector3 center;
		Vector3 orientations[3];
		Vector3 size;				//各半径
	};

	OBB obb_;

	//過去情報
	WorldTransform preWorld_;

	//world行列
	Matrix4x4 OBBM_;
	//worldの逆行列
	Matrix4x4 inverseM_;
	AABB aabb_;
};