#include "OBBCollider.h"
#include<imgui.h>
#include"SphereCollider/SphereCollider.h"


bool OBBCollider::isDraw_ = true;

void OBBCollider::Initialize(const std::string& tag, const WorldTransform& parent)
{
	//インスタンシングの初期化
	InstancingGameObject::Initialize("box");
	//親子関係設定
	world_.parent_=(&parent);
	//コライダーのタグ設定
	colliderTag_ = tag;

	//画像を切る
	IMM_->SetEnableTexture(tag_, false);
	//ワイヤーフレーム表示
	//IMM_->SetFillMode(tag_, FillMode::kWireFrame);
	//影の削除
	IMM_->SetEnableShader(tag_, false);
	//透明度設定
	IMM_->SetAlpha(tag_, alpha_);
}

void OBBCollider::Initialize(const std::string& tag)
{
	//院スタン寝具の初期化
	InstancingGameObject::Initialize("box");

	//コライダーのタグ設定
	colliderTag_ = tag;

	//画像を切る
	IMM_->SetEnableTexture(tag_, false);
	//ワイヤーフレーム表示
	IMM_->SetFillMode(tag_, FillMode::kWireFrame);
	//影の削除
	IMM_->SetEnableShader(tag_, false);
	//透明度設定
	IMM_->SetAlpha(tag_, alpha_);
}

void OBBCollider::Update()
{
	preWorld_ = world_;
	/*Matrix4x4 rotateXM = MakeRotateAxisAngle({ 1,0,0 }, rotation_.x);
	Matrix4x4 rotateYM = MakeRotateAxisAngle({ 0,1,0 }, rotation_.y);
	Matrix4x4 rotateZM = MakeRotateAxisAngle({ 0,0,1 }, rotation_.z);

	Matrix4x4 rotateM = rotateXM * (rotateYM * rotateZM);

	Matrix4x4 scaleM = MakeScaleMatrix(world_.scale_);

	Matrix4x4 tlansM = MakeTranslateMatrix(world_.translate_);

	Matrix4x4 worldM = scaleM * (rotateM * tlansM);

	if (&world_.GetParent() != nullptr) {
		Matrix4x4 pare = world_.parent_->matWorld_;
		world_.matWorld_ =  worldM * pare;
	}
	else {
		world_.matWorld_ = worldM;
	}*/

	world_.UpdateMatrix();


#pragma region OBBのワールド行列をスケールなしで作成
	//回転量取得
	Vector3 rotate = GetAllRotate(world_);

	//回転行列
	Matrix4x4 rotateM = MakeRotateXMatrix(rotate.x) * (MakeRotateYMatrix(rotate.y) * MakeRotateZMatrix(rotate.z));
	//座標行列
	Matrix4x4 translateM = MakeTranslateMatrix(world_.GetMatWorldTranslate());
	//スケールは使わない（sizeで使う
	Matrix4x4 scaleM = MakeIdentity4x4();
	//OBBのworld行列生成
	OBBM_ = scaleM * (rotateM * translateM);

	//逆行列
	inverseM_ = Inverse(OBBM_);

	//すべてのスケールからサイズ取得
	Vector3 size = GetAllScale(world_);

	//AABB取得
	aabb_ = { .minV = -size,.maxV = size };
#pragma endregion

}

void OBBCollider::Draw()
{
#ifdef _DEBUG
	if (isDraw_) {
		InstancingModelManager::GetInstance()->SetData(tag_, world_,color_);
	}
#endif // _DEBUG
}

void OBBCollider::Debug(const char* name)
{

#ifdef _DEBUG
	ImGui::Begin(name);
	ImGui::DragFloat3("trans", &world_.translate_.x, 0.10f);
	ImGui::DragFloat3("rotation", &world_.rotate_.x, 0.01f);
	ImGui::DragFloat3("scale", &world_.scale_.x, 0.01f);

	ImGui::ColorEdit4("hit color", &hitColor.x);
	ImGui::Checkbox("isDraw", &isDraw_);
	ImGui::End();
#endif // _DEBUG


}

bool OBBCollider::IsCollision(SphereCollider* collider,Vector3& backVec)
{

	//スフィアコライダーの座標をOBBのローカル空間に出る
	Vector3 sphereLocal = Transform(collider->GetWorld().GetMatWorldTranslate(), inverseM_);
	
	
	//Sphere取得
	Sphere sphere = { sphereLocal,collider->GetRadius()};

	//当たり判定
	Vector3 saikin{};
	if (InCollision(aabb_, sphere, saikin)) {


		//OBBLocalPosCange
		saikin = Transform(saikin, OBBM_);

		//mosionajiiti
		if (world_.GetMatWorldTranslate() == saikin) {
			//スフィアコライダーの座標をOBBのローカル空間に出る
			sphereLocal = Transform(preWorld_.GetMatWorldTranslate(), inverseM_);
			//Sphere取得
			sphere = { sphereLocal,collider->GetRadius()};
			InCollision(aabb_, sphere, saikin);

			saikin = Transform(saikin, OBBM_);

			Vector3 velo = preWorld_.GetMatWorldTranslate() - saikin;
			velo.SetNormalize();
			velo *= collider->GetRadius();

			backVec = velo;

		}
		else {
			///押し出しベクトルを利用して計算
			//最近接点から円の中心点への向きベクトルを算出
			Vector3 velo = collider->GetWorld().GetMatWorldTranslate() - saikin;
			//正規化

			Vector3 norVe = velo;
			norVe.SetNormalize();
			//半径分伸ばす
			norVe *= collider->GetRadius();

			//渡す
			backVec = norVe - velo;

			
		}


		////最近接点描画
		WorldTransform sWo;
		sWo.translate_ = saikin;
		sWo.scale_ = { 0.1f,0.1f,0.1f };
		sWo.UpdateMatrix();
		IMM_->SetData("sphere", sWo);

		SetColor(true);

		collider->SetColor(true);

		backVec *= -1;
		return true;



	}
	else {
		//最近接点描画
		WorldTransform sWo;
		sWo.translate_ = Transform(saikin, OBBM_);
		sWo.scale_ = { 0.1f,0.1f,0.1f };
		sWo.UpdateMatrix();
		IMM_->SetData("sphere", sWo);


		//色の変更
		SetColor(false);
		collider->SetColor(false);

		return false;
	}
	 
}

bool OBBCollider::IsCollision(const Segment& seg) {



	//各OBBローカル上に作成
	Vector3 localOri = Transform(seg.origin, inverseM_);
	Vector3 localEnd = Transform(seg.origin+seg.diff, inverseM_);

	//セグメント取得
	Segment lSeg = { localOri,localEnd - localOri };

	return InCollision(aabb_, seg);
}


void OBBCollider::SetColor(bool hit)
{
	if (hit) {
		color_ = hitColor;
	}
	else {
		color_ = normalColor;
	}
}

void OBBCollider::UpdateMatrix()
{
	world_.UpdateMatrix();

#pragma region OBBのワールド行列をスケールなしで作成
	//回転量取得
	Vector3 rotate = GetAllRotate(world_);

	//回転行列
	Matrix4x4 rotateM = MakeRotateXMatrix(rotate.x) * (MakeRotateYMatrix(rotate.y) * MakeRotateZMatrix(rotate.z));
	//座標行列
	Matrix4x4 translateM = MakeTranslateMatrix(world_.GetMatWorldTranslate());
	//スケールは使わない（sizeで使う
	Matrix4x4 scaleM = MakeIdentity4x4();
	//OBBのworld行列生成
	OBBM_ = scaleM * (rotateM * translateM);

	//逆行列
	inverseM_ = Inverse(OBBM_);

	//すべてのスケールからサイズ取得
	Vector3 size = GetAllScale(world_);

	//AABB取得
	aabb_ = { .minV = -size,.maxV = size };
#pragma endregion
}
