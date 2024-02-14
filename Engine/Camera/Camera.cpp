#include"Camera.h"
#include"WinApp/WinApp.h"
#include<imgui.h>
#include<numbers>

Camera::Camera() {}

Camera::~Camera() {}

void Camera::Initialize() {

	mainCamera_.Initialize();
	CameraMotionSupport_.Initialize();
	FeaturedWorldTransform_ = nullptr;

	//カメラの初期距離を設定
	mainCamera_.translate_.z = rangeCameraFeaturedPoint;

	//ポイントカメラの回転の設定
	CameraMotionSupport_.rotate_ = { 0.6f,0.0f,0.0f };

	//親子関係の処理
	mainCamera_.parent_ = (&CameraMotionSupport_);

	//行列更新
	CameraMotionSupport_.UpdateMatrix();
	mainCamera_.UpdateMatrix();

	//各種更新に必要な処理
	view_ = Inverse(mainCamera_.matWorld_);
	projection_ = MakePerspectiveFovMatrix(0.45f, (float)WindowApp::kClientWidth / (float)WindowApp::kClientHeight, 0.1f, FarZ);
	viewProjection_ = view_ * projection_;
}

void Camera::Update() {

	isCollision_ = false;

	//回転量Xの制限
	if (CameraMotionSupport_.rotate_.x < minRotateX) {
		CameraMotionSupport_.rotate_.x = minRotateX;
	}
	else if (CameraMotionSupport_.rotate_.x > maxRotateX) {
		CameraMotionSupport_.rotate_.x = maxRotateX;
	}

	//一週以上している場合制限
	if (CameraMotionSupport_.rotate_.y > (float)std::numbers::pi * 2.0f) {
		CameraMotionSupport_.rotate_.y -= (float)std::numbers::pi * 2.0f;
	}
	if (CameraMotionSupport_.rotate_.y < -(float)std::numbers::pi * 2.0f) {
		CameraMotionSupport_.rotate_.y += (float)std::numbers::pi * 2.0f;
	}

	//注目するものがあるとき
	if (FeaturedWorldTransform_) {
		//座標のみ取得するフラグが起動しているとき
		if (isOnlyGetPosition) {
			CameraMotionSupport_.translate_ = camerapos_ + FeaturedWorldTransform_->GetMatWorldTranslate();
			CameraMotionSupport_.UpdateMatrix();
		}
		else {
			CameraMotionSupport_.translate_ = camerapos_;
			CameraMotionSupport_.UpdateMatrix();
			//起動していないとき行列をかけて親子関係処理
			CameraMotionSupport_.matWorld_ = CameraMotionSupport_.matWorld_ * FeaturedWorldTransform_->matWorld_;
		}
	}


	//メインカメラの更新
	mainCamera_.UpdateMatrix();

	//各種更新に必要な処理
	view_ = Inverse(mainCamera_.matWorld_);
	projection_ = MakePerspectiveFovMatrix(0.45f, (float)WindowApp::kClientWidth / (float)WindowApp::kClientHeight, 0.1f, FarZ);
	viewProjection_ = view_ * projection_;

#pragma region Segment設定
	//カメラ位置計算
	segment_.origin = mainCamera_.GetMatWorldTranslate();
	//距離計算
	segment_.diff = TransformNormal({ 0,0,1 }, mainCamera_.matWorld_);
	segment_.diff.SetNormalize();
	segment_.diff *= mainCamera_.translate_.z * -1.0f;
#pragma endregion


}

void Camera::DrawDebugWindow(const char* name) {
#ifdef _DEBUG

	if (ImGui::BeginMenu(name)) {
		ImGui::Text("mainCamera");
		ImGui::DragFloat3("mainC pos", &camerapos_.x, 0.01f);
		ImGui::DragFloat("targetFar", &mainCamera_.translate_.z,0.1f);
		ImGui::DragFloat3("mainC rotate", &mainCamera_.rotate_.x, 0.01f);
		ImGui::DragFloat3("mainC scale", &mainCamera_.scale_.x, 0.01f);

		ImGui::Text("LocationCenterMotion");
		ImGui::DragFloat3("PCM pos", &CameraMotionSupport_.translate_.x, 0.01f);
		ImGui::DragFloat3("PCM rotate", &CameraMotionSupport_.rotate_.x, 0.01f);
		ImGui::DragFloat3("PCM scale", &CameraMotionSupport_.scale_.x, 0.01f);
		ImGui::Checkbox("isOnlyGetPosition", &isOnlyGetPosition);
		ImGui::EndMenu();
	}

#endif // _DEBUG

}

void Camera::UpdateMatrixes() {
	//行列更新
	CameraMotionSupport_.UpdateMatrix();
	mainCamera_.UpdateMatrix();

	//各種更新に必要な処理
	view_ = Inverse(mainCamera_.matWorld_);
	projection_ = MakePerspectiveFovMatrix(0.45f, (float)WindowApp::kClientWidth / (float)WindowApp::kClientHeight, 0.1f, FarZ);
	viewProjection_ = view_ * projection_;
}

void Camera::IsCollision(OBBCollider* obb)
{


	//カメラのレイとの処理
	//t定義
	float t = 0;
	//分割量
	float division = farFeaturedPos_;

	//注目点
	Vector3 featuredPos = GetFeaturedWorld().GetMatWorldTranslate();

	while (t <= 1.0f) {
		//Zが設定値のmatWorldを生成
		WorldTransform world = mainCamera_;
		world.translate_.z = farFeaturedPos_;
		world.UpdateMatrix();
		//カメラ位置
		Vector3 cameraPos = world.GetMatWorldTranslate();
		//距離だけは設定場所に


		//目標地点からカメラまで円をイージング
		Vector3 pos = {
			cameraPos.x * (t)+featuredPos.x * (1.0f - t),
			cameraPos.y * (t)+featuredPos.y * (1.0f - t),
			cameraPos.z * (t)+featuredPos.z * (1.0f - t),
		};

		//円コライダーを仮生成
		SphereCollider* sphere = new SphereCollider();
		sphere->Initialize("sphere");
		sphere->SetTranslate(pos);
		sphere->SetRadius(0.1f);
		sphere->Update();

		//あたったかチェック
		Vector3 temp;
		if (sphere->IsCollision(*obb, temp, 1)) {
			///あたったら再配置
			//注目点とあたった点との距離計算
			float length = Length(featuredPos - pos);
			if (mainCamera_.translate_.z < -length + direction) {
				mainCamera_.translate_.z = -length + direction;
				UpdateMatrixes();
			}
			//obb->SetColor(true);
			isCollision_ = true;
			//終了
			return;
		}
		else {

			if (!isCollision_) {
				mainCamera_.translate_.z = farFeaturedPos_;
			}
			//obb->SetColor(false);
		}

		t += 1.0f / -division;

		delete sphere;
	}
}

void Camera::SetTarget(const WorldTransform* parent) {
	FeaturedWorldTransform_ = parent;
}

void Camera::SetCameraDirection(const float farZ) {
	farFeaturedPos_ = farZ;
	mainCamera_.translate_.z = farZ;
}
