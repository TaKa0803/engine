#include"ParticleEmiter.h"
#include"functions/function.h"
#include"SRVManager/SRVManager.h"
#include<imgui.h>

ParticleEmiter::ParticleEmiter() {
}

ParticleEmiter::~ParticleEmiter() {
	delete particlegraphics_;
	vertexResource_->Release();
	indexResource_->Release();
	transformationMatrixResource_->Release();
	materialResource_->Release();
}

ParticleEmiter* ParticleEmiter::Create2D(const Camera* camera, int texture, const int occurrenceMaxCount, const Vector2 spriteSize, const Vector2 Rect, const Vector2 anchor) {

	DirectXFunc* DXF = DirectXFunc::GetInstance();


#pragma region VertexResourceとVertexBufferViewを用意
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	//Sprite用の頂点リソースを作る
	ID3D12Resource* vertexResource = CreateBufferResource(DXF->GetDevice(), sizeof(VertexData) * 4);
	//リソース用の先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 4;
	//頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);
#pragma endregion
#pragma region 頂点データを設定する
	VertexData* vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//一枚目の三角形
	Vector2 minv = { 1* (-anchor.x),1 * (-anchor.y) };

	Vector2 maxV = { 1 * (1 - anchor.x),1 * (1 - anchor.y) };

	Vector2 maxTex = { Rect.x / spriteSize.x,Rect.y / spriteSize.x };

	vertexData[0].position = { minv.x,maxV.y,0.0f,1.0f };
	vertexData[0].texcoord = { 0.0f,maxTex.y };
	vertexData[0].normal = { 0.0f,0.0f,-1.0f };

	vertexData[1].position = { minv.x,minv.y,0.0f,1.0f };
	vertexData[1].texcoord = { 0.0f,0.0f };
	vertexData[1].normal = { 0.0f,0.0f,-1.0f };

	vertexData[2].position = { maxV.x,maxV.y,0.0f,1.0f };
	vertexData[2].texcoord = maxTex;
	vertexData[2].normal = { 0.0f,0.0f,-1.0f };

	vertexData[3].position = { maxV.x,minv.y,0.0f,1.0f };
	vertexData[3].texcoord = { maxTex.x,0.0f };
	vertexData[3].normal = { 0.0f,0.0f,-1.0f };


#pragma endregion

#pragma region Indexデータ
	ID3D12Resource* indexResource = CreateBufferResource(DXF->GetDevice(), sizeof(uint32_t) * 6);
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};

	//リソースの先頭アドレスから使う
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス６つ分のサイズ
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;
	//インデックス
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	uint32_t* indexDataSprite = nullptr;
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSprite));
	indexDataSprite[0] = 1;
	indexDataSprite[1] = 3;
	indexDataSprite[2] = 0;

	indexDataSprite[3] = 3;
	indexDataSprite[4] = 2;
	indexDataSprite[5] = 0;

#pragma endregion

	ParticleEmiter* particleInstancingData = new ParticleEmiter();
	particleInstancingData->Initialize(camera,texture, occurrenceMaxCount, vertexResource, vertexBufferView,indexResource,indexBufferView, true);

	return particleInstancingData;

}

ParticleEmiter* ParticleEmiter::Create3D(const Camera* camera, int texture, const int occurrenceMaxCount,const Vector2 spriteSize, const Vector2 Rect, const Vector2 anchor) {
	DirectXFunc* DXF = DirectXFunc::GetInstance();


#pragma region VertexResourceとVertexBufferViewを用意
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	//Sprite用の頂点リソースを作る
	ID3D12Resource* vertexResource = CreateBufferResource(DXF->GetDevice(), sizeof(VertexData) * 4);
	//リソース用の先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 4;
	//頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);
#pragma endregion
#pragma region 頂点データを設定する
	VertexData* vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//一枚目の三角形
	Vector2 minV = { 1 * (-anchor.x),1 * (-anchor.y) };

	Vector2 maxV = { 1 * (1 - anchor.x),1 * (1 - anchor.y) };

	Vector2 maxTex = { Rect.x / spriteSize.x,Rect.y / spriteSize.x };

	vertexData[0].position = { minV.x,0.0f,minV.y,1.0f };
	vertexData[0].texcoord = { 0.0f,maxTex.y };
	vertexData[0].normal = { 0.0f,0.0f,-1.0f };

	vertexData[1].position = { minV.x,0.0f,maxV.y,1.0f };
	vertexData[1].texcoord = { 0.0f,0.0f };
	vertexData[1].normal = { 0.0f,0.0f,-1.0f };

	vertexData[2].position = { maxV.x,0.0f,maxV.y,1.0f };
	vertexData[2].texcoord =  { maxTex.x,0.0f };
	vertexData[2].normal = { 0.0f,0.0f,-1.0f };

	vertexData[3].position = { maxV.x,0.0f,minV.y,1.0f };
	vertexData[3].texcoord = maxTex;
	vertexData[3].normal = { 0.0f,0.0f,-1.0f };


#pragma endregion

#pragma region Indexデータ
	ID3D12Resource* indexResource = CreateBufferResource(DXF->GetDevice(), sizeof(uint32_t) * 6);
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};

	//リソースの先頭アドレスから使う
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス６つ分のサイズ
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;
	//インデックス
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	uint32_t* indexDataSprite = nullptr;
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSprite));
	indexDataSprite[0] = 0;
	indexDataSprite[1] = 1;
	indexDataSprite[2] = 2;

	indexDataSprite[3] = 3;
	indexDataSprite[4] = 0;
	indexDataSprite[5] = 2;

#pragma endregion

	ParticleEmiter* particleInstancingData = new ParticleEmiter();
	particleInstancingData->Initialize(camera,texture, occurrenceMaxCount, vertexResource, vertexBufferView,indexResource,indexBufferView);

	return particleInstancingData;

}

void ParticleEmiter::Update() {
	for (auto& particle : particles_) {

		//カメラ
		Vector3 c_pos = camera_->GetMainCamera().GetMatWorldTranslate();
		//対象物
		Vector3 e_pos = particle->world_.GetMatWorldTranslate();

		//対象物からカメラへの方向
		Vector3 veloE2C = c_pos - e_pos;

		ImGui::Begin("c-e dire");
		ImGui::DragFloat3("velo", &veloE2C.x);
		ImGui::End();

	}
}


void ParticleEmiter::Initialize(const Camera* camera, int32_t texture, int32_t occurrenceMaxCount, ID3D12Resource* vertexResource, D3D12_VERTEX_BUFFER_VIEW vertexBufferView, ID3D12Resource* indexResource, D3D12_INDEX_BUFFER_VIEW indexBufferView, bool is2D) {

	is2D_ = is2D;

	DXF_ = DirectXFunc::GetInstance();

	particlegraphics_ = new ParticleGraphics();
	particlegraphics_->Initialize(DXF_->GetDevice());

	camera_ = camera;

	texture_ = texture;

	vertexResource_ = vertexResource;
	vertexBufferView_ = vertexBufferView;

	indexResource_=indexResource;

	indexBufferView_=indexBufferView;

#pragma region Transform周りを作る	
	transformationMatrixResource_ = CreateBufferResource(DXF_->GetDevice(), sizeof(Particle4GPU) * occurrenceMaxCount);
	//データを書き込む
	//書き込むためのアドレスを取得
	transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&particle4GPUData_));
	for (uint32_t index = 0; index < (uint32_t)occurrenceMaxCount; ++index) {
		particle4GPUData_[index].WVP = MakeIdentity4x4();
		particle4GPUData_[index].World = MakeIdentity4x4();
		particle4GPUData_[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}
#pragma endregion
#pragma region Material作成
	//Sprite用のマテリアルリソース
	materialResource_ = CreateBufferResource(DXF_->GetDevice(), sizeof(Material));
	//マテリアルにデータを書き込む

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = false;
	materialData_->uvTransform = MakeIdentity4x4();
	materialData_->enableTexture = true;
	materialData_->enableHalfLambert = false;
#pragma endregion
#pragma region instancing関係のデータ
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingDesc{};
	instancingDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingDesc.Buffer.FirstElement = 0;
	instancingDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingDesc.Buffer.NumElements = occurrenceMaxCount;
	instancingDesc.Buffer.StructureByteStride = sizeof(Particle4GPU);

	SRVManager* SRVM = SRVManager::GetInstance();
	instancingHandleNum = SRVM->CreateSRV(transformationMatrixResource_, nullptr, instancingDesc);
#pragma endregion


}




void ParticleEmiter::Draw2D(int texture) {

	//3D用に生成してるときにこれを呼び出しでエラー
	if (!is2D_) {
		assert(false);
	}

	particlegraphics_->PreDraw(DXF_->GetCMDList());

	uvWorld_.UpdateMatrix();

	//uvTransform更新
	materialData_->uvTransform = uvWorld_.matWorld_;

	//生きている数と番号チェック
	int index = 0;
	for (auto& particle : particles_) {
		particle->world_.UpdateMatrix();



		//ワールド更新
		Matrix4x4 World = particle->world_.matWorld_;


		//スプライト用データ
		Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(WindowApp::kClientWidth), float(WindowApp::kClientHeight), 0.0f, 100.0f);
		Matrix4x4 VPSprite = viewMatrixSprite * projectionMatrixSprite;
		Matrix4x4 WVP = World * VPSprite;
		//データ代入
		particle4GPUData_[index].WVP = WVP;
		particle4GPUData_[index].World = World;
		particle4GPUData_[index].color = particle->color;

		index++;
	}
	particles_.clear();

	//Spriteの描画
	DXF_->GetCMDList()->IASetVertexBuffers(0, 1, &vertexBufferView_);	//VBVを設定			
	DXF_->GetCMDList()->IASetIndexBuffer(&indexBufferView_);//IBVを設定
	//形状を設定、PSOに設定しているものとはまた別、同じものを設定すると考えておけばいい
	DXF_->GetCMDList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//マテリアルCBufferの場所を設定
	DXF_->GetCMDList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//TransformationMatrixCBufferの場所を設定
	//DXF->GetCMDList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());

	//インスタンシング座標設定
	DXF_->GetCMDList()->SetGraphicsRootDescriptorTable(1, SRVManager::GetInstance()->GetTextureDescriptorHandle(instancingHandleNum));

	//画像が指定されたらそれを表示
	if (texture != -1) {
		DXF_->GetCMDList()->SetGraphicsRootDescriptorTable(2, SRVManager::GetInstance()->GetTextureDescriptorHandle(texture));
	}
	else {
		//されていないのでそのまま表示
		DXF_->GetCMDList()->SetGraphicsRootDescriptorTable(2, SRVManager::GetInstance()->GetTextureDescriptorHandle(texture_));
	}
	//描画！！（DrawCall
	DXF_->GetCMDList()->DrawIndexedInstanced(6, index, 0, 0, 0);
}

void ParticleEmiter::Draw3D(int texture) {

	//2D用に生成してるときにこれを呼び出しでエラー
	if (is2D_) {
		assert(false);
	}

	particlegraphics_->PreDraw(DXF_->GetCMDList());

	

	uvWorld_.UpdateMatrix();

	//uvTransform更新
	materialData_->uvTransform = uvWorld_.matWorld_;

	//生きている数と番号チェック
	int index = 0;
	for (auto& particle : particles_) {


		//カメラ
		Vector3 c_pos = camera_->GetMainCamera().GetMatWorldTranslate();
		//対象物
		Vector3 e_pos = particle->world_.GetMatWorldTranslate();
		
		//対象物からカメラへの方向
		Vector3 veloE2C = c_pos - e_pos;


		

		float xzleng = sqrtf(veloE2C.x * veloE2C.x + veloE2C.z * veloE2C.z);

		float RX = GetYRotate(Vector2(xzleng, veloE2C.y));
		particle->world_.rotate_.x = RX;

		
		float RY = GetYRotate(Vector2(veloE2C.x, veloE2C.z));
		particle->world_.rotate_.y = RY;


		particle->world_.UpdateMatrix();
		//ワールド更新
		Matrix4x4 World = particle->world_.matWorld_;

		//Matrix4x4 rotateM = DirectionToDirection(e_pos, c_pos);

		//Matrix4x4 World = MakeScaleMatrix(particle->world_.scale_) * (rotateM * MakeTranslateMatrix(particle->world_.translate_));

		//スプライト用データ
		Matrix4x4 WVP = World * camera_->GetViewProjectionMatrix();
		//データ代入
		particle4GPUData_[index].WVP = WVP;
		particle4GPUData_[index].World = World;
		particle4GPUData_[index].color = particle->color;

		index++;
	}
	particles_.clear();

	//Spriteの描画
	DXF_->GetCMDList()->IASetVertexBuffers(0, 1, &vertexBufferView_);	//VBVを設定			
	DXF_->GetCMDList()->IASetIndexBuffer(&indexBufferView_);//IBVを設定
	//形状を設定、PSOに設定しているものとはまた別、同じものを設定すると考えておけばいい
	DXF_->GetCMDList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//マテリアルCBufferの場所を設定
	DXF_->GetCMDList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//TransformationMatrixCBufferの場所を設定
	//DXF->GetCMDList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());

	//インスタンシング座標設定
	DXF_->GetCMDList()->SetGraphicsRootDescriptorTable(1, SRVManager::GetInstance()->GetTextureDescriptorHandle(instancingHandleNum));

	//画像が指定されたらそれを表示
	if (texture != -1) {
		DXF_->GetCMDList()->SetGraphicsRootDescriptorTable(2, SRVManager::GetInstance()->GetTextureDescriptorHandle(texture));
	}
	else {
		//されていないのでそのまま表示
		DXF_->GetCMDList()->SetGraphicsRootDescriptorTable(2, SRVManager::GetInstance()->GetTextureDescriptorHandle(texture_));
	}
	//描画！！（DrawCall
	DXF_->GetCMDList()->DrawIndexedInstanced(6, index, 0, 0, 0);

}

void ParticleEmiter::DebugImGui(const char* name) {

#ifdef _DEBUG
	ImGui::Begin(name);
	ImGui::ColorEdit4("color", &materialData_->color.x);
	ImGui::DragFloat3("rotate", &rotate_.x, 0.01f);
	//改行
	ImGui::Text("");
	ImGui::Text("UV");
	ImGui::DragFloat2("uv pos", &uvWorld_.translate_.x, 0.1f);
	ImGui::DragFloat("uv rotate", &uvWorld_.rotate_.z, 0.1f);
	ImGui::DragFloat2("uv color", &uvWorld_.scale_.x, 0.1f);
	//改行	
	ImGui::Text("");
	BlendMode blend = particlegraphics_->GetBlendMode();
	const char* items[] = { "None","Normal","Add","Subtract","Multiply","Screen" };
	int currentItem = static_cast<int>(blend);
	if (ImGui::Combo("blendmode", &currentItem, items, IM_ARRAYSIZE(items))) {
		blend = static_cast<BlendMode>(currentItem);
	}
	particlegraphics_->SetBlendMode(blend);

	ImGui::Text("");
	ImGui::Text("SetDatas");
	for (auto& particle : particles_) {
		ImGui::Text("pos : %4.1f / %4.1f / %4.1f", particle->world_.translate_.x, particle->world_.translate_.y, particle->world_.translate_.z);
		ImGui::Text("color : %4.1f / %4.1f / %4.1f / %4.1f", particle->color.x, particle->color.y, particle->color.z, particle->color.w);
		ImGui::Text("");
	}


	ImGui::End();

#endif // _DEBUG


}
