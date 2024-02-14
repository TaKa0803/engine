#pragma once
#include"WorldTransform/WorldTransform.h"
#include"ColliderOBB/OBBCollider.h"
#include"SphereCollider/SphereCollider.h"
#include"struct.h"


class Camera {

public:

	Camera();
	~Camera();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// デバッグウィンドウ表示
	/// </summary>
	/// <param name="name">ウィンドウ名</param>
	void DrawDebugWindow(const char* name);

	/// <summary>
	/// マトリックス更新
	/// </summary>
	void UpdateMatrixes();

	//カメラとobb判定
	void IsCollision(OBBCollider*obb);
public:

	/// <summary>
	/// カメラが注目しているものの座標取得
	/// </summary>
	/// <returns>translateを返却</returns>
	const Vector3& GetFeaturedPos()const { return CameraMotionSupport_.translate_; }

	const WorldTransform& GetFeaturedWorld()const { return *FeaturedWorldTransform_; }

	/// <summary>
	/// メインカメラのWorldTransform取得
	/// </summary>
	/// <returns>WorldTransformを返却</returns>
	const WorldTransform& GetMainCamera()const { return mainCamera_; }

	/// <summary>
	/// ViewProjectionを取得
	/// </summary>
	/// <returns>ViewProjectionの4x4行列を返却</returns>
	const Matrix4x4& GetViewProjectionMatrix() const { return viewProjection_; }

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	float GetPCameraR_Y() { return CameraMotionSupport_.rotate_.y; }

	/// <summary>
	/// カメラの視線取得
	/// </summary>
	/// <returns></returns>
	const Segment GetSegment() { return segment_; }
public:
	//ターゲット設定
	void SetTarget(const WorldTransform* parent);

	//カメラのターゲットとの距離設定
	void SetCameraDirection(const float far);

	//メインカメラ座標
	void SetMainCameraPos(const Vector3& pos) { camerapos_ = pos; }

	//カメラ野注目手座標設定
	void SetCameraFeaturedPos(const Vector3& pos) { CameraMotionSupport_.translate_ = pos; }

	//カメラの回転量Ysettei 
	void AddCameraR_Y(float y) { CameraMotionSupport_.rotate_.y += y; }

	void AddCameraR_X(float x) { CameraMotionSupport_.rotate_.x += x; }

private:
	//カメラ本体
	WorldTransform mainCamera_;

	//カメラの平行移動
	Vector3 camerapos_{};

	//カメラの注目点があるときの回転処理用
	WorldTransform CameraMotionSupport_;

	//カメラの回転量
	const float maxRotateX = 1.0f;
	const float minRotateX = 0.01f;

	//注目しているWorldT
	const WorldTransform* FeaturedWorldTransform_ = nullptr;

	//カメラの注目品との初期距離
	const float rangeCameraFeaturedPoint = -50;

	//完全追従するか座標のみ取得するか
	bool isOnlyGetPosition = true;

	//描画距離
	float FarZ = 1000.0f;

	//ビュー
	Matrix4x4 view_ = MakeIdentity4x4();

	//プロジェクション
	Matrix4x4 projection_ = MakeIdentity4x4();

	//VP
	Matrix4x4 viewProjection_ = MakeIdentity4x4();

	//注目点都の値
	float farFeaturedPos_ = -50;

	//カメラの視線
	Segment segment_;

	//あたった時の追加で寄せる量
	float direction = 0;

	bool isCollision_ = false;
};