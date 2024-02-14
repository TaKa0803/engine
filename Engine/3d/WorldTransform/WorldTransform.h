#pragma once
#include"struct.h"
#include"Math/Matrix.h"


class WorldTransform {


public://変数

	//座標
	Vector3 translate_ = { 0.0f,0.0f,0.0f };

	//回転
	Vector3 rotate_ = { 0.0f,0.0f,0.0f };

	//拡縮
	Vector3 scale_ = { 1.0f,1.0f,1.0f };

	//ワールド行列
	Matrix4x4 matWorld_ = MakeIdentity4x4();

	//親
	const WorldTransform* parent_ = nullptr;

public:

	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	Matrix4x4 UpdateMatrix();

	/// <summary>
	/// パラメータデバッグ表示
	/// </summary>
	void DrawDebug(const char*name);


#pragma region ゲッター
public://Getter

	/// <summary>
	/// ローカル座標取得
	/// </summary>
	/// <returns>ローカルtranslate</returns>
	const Vector3& GetTranslate() const { return translate_; }

	/// <summary>
	/// ローカル回転取得
	/// </summary>
	/// <returns>ローカルrotate</returns>
	const Vector3& GetRotate()const { return rotate_; }

	/// <summary>
	/// ローカル拡縮取得
	/// </summary>
	/// <returns>ローカルscale</returns>
	const Vector3& GetScale()const { return scale_; }

	/// <summary>
	/// matWorldのTranslate取得
	/// </summary>
	/// <returns>ワールドのtranslate</returns>
	const Vector3 GetMatWorldTranslate() const {
		Vector3 matTranslation = {
			matWorld_.m[3][0],
			matWorld_.m[3][1],
			matWorld_.m[3][2]
		};
		return matTranslation;
	}

public:

	void SetScale(float radius) { scale_ = { radius,radius,radius }; }

	void SetScale(Vector3 scale) { scale_ = scale; }
#pragma endregion


};