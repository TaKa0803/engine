#include "InstancingModel.h"

#include"Log/Log.h"
#include"functions/function.h"
#include"SRVManager/SRVManager.h"
#include"TextureManager/TextureManager.h"
#include"SingleModelManager/ModelManager.h"
#include"functions/function.h"

#include<imgui.h>
/*
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
*/


InstancingPSO* InstancingModel::pso_ =new InstancingPSO();

InstancingModel::~InstancingModel() {
	
	delete pso_;

	vertexData_->Release();
	wvpResource_->Release();
	//materialResource_->Release();
	//directionalLightResource_->Release();
}

InstancingModel* InstancingModel::CreateFromOBJ(const std::string& directory,const std::string& filePath, int instancingNum) {
	DirectXFunc* DXF = DirectXFunc::GetInstance();

#pragma region モデル
	ModelManager* mManager = ModelManager::GetInstance();

	//もでるよみこみ
	//Assimp::Importer importer;
	std::string fileFullPath = directory + "/" + filePath+"/"+filePath+".obj";
	

	ModelData modeltea = LoadObjFile(directory,filePath);

	//頂点データ
	ID3D12Resource* vertexRtea = CreateBufferResource(DXF->GetDevice(), sizeof(VertexData) * modeltea.vertices.size());
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewtea{};
	vertexBufferViewtea.BufferLocation = vertexRtea->GetGPUVirtualAddress();
	vertexBufferViewtea.SizeInBytes = UINT(sizeof(VertexData) * modeltea.vertices.size());
	vertexBufferViewtea.StrideInBytes = sizeof(VertexData);

	VertexData* vertexDatatea = nullptr;
	vertexRtea->Map(0, nullptr, reinterpret_cast<void**>(&vertexDatatea));
	std::memcpy(vertexDatatea, modeltea.vertices.data(), sizeof(VertexData) * modeltea.vertices.size());


	
#pragma endregion



	InstancingModel* model = new InstancingModel();
	model->Initialize(modeltea.material.textureFilePath, UINT(modeltea.vertices.size()),instancingNum, vertexRtea, vertexBufferViewtea);




	return model;
}

void InstancingModel::PreUpdate() {
	worlds_.clear();
}

void InstancingModel::AddWorld(const WorldTransform& world, const Vector4& color) {
	//データをコピー
	InstancingData worl = { world,color };

	std::unique_ptr<InstancingData>newWorld = std::make_unique<InstancingData>(worl);
	//追加
	worlds_.push_back(std::move(newWorld));
	
}

void InstancingModel::Draw(const Matrix4x4& viewProjection, int texture) {

	int index = 0;
	for (auto& world : worlds_) {
		Matrix4x4 worldM = world->world.matWorld_;

		Matrix4x4 WVP = worldM * viewProjection;

		wvpData_[index].WVP = WVP;
		wvpData_[index].World = worldM;
		wvpData_[index].color = world->color;

		index++;
	}

	if (index > instancingNum_) {
		//indexが初期作成量よりおおい
		assert(false);
	}

	if (index > 0) {
		pso_->PreDraw(DXF_->GetCMDList(),fillMode_,blendMode_);

		uvWorld_.UpdateMatrix();
		materialData_->uvTransform = uvWorld_.matWorld_;



		DXF_->GetCMDList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
		//形状を設定、PSOに設定しているものとはまた別、同じものを設定すると考えておけばいい
		DXF_->GetCMDList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		//マテリアルCBufferの場所を設定
		DXF_->GetCMDList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
		//
		DXF_->GetCMDList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());

		if (setTexture_ == -1) {
			if (texture == -1) {
				DXF_->GetCMDList()->SetGraphicsRootDescriptorTable(2, texture_);
			}
			else {
				//SRVのDescriptorTableの先頭を設定。２はParameter[2]である。
				DXF_->GetCMDList()->SetGraphicsRootDescriptorTable(2, SRVManager::GetInstance()->GetTextureDescriptorHandle(texture));
			}
		}
		else {
			DXF_->GetCMDList()->SetGraphicsRootDescriptorTable(2, SRVManager::GetInstance()->GetTextureDescriptorHandle(setTexture_));
		}
		DXF_->GetCMDList()->SetGraphicsRootDescriptorTable(1, instancingHandle_);

		//描画！		
		DXF_->GetCMDList()->DrawInstanced(point_, index, 0, 0);

	}
}

void InstancingModel::Debug(const char* name)
{
	bool uselight = materialData_->enableLighting;
	bool useHalfLam = materialData_->enableHalfLambert;
	bool useTex = materialData_->enableTexture;
	

	Vector4 color = materialData_->color;

	if (ImGui::BeginMenu(name)) {

		ImGui::Text("Material");
		ImGui::Checkbox("lighting", &uselight);
		ImGui::Checkbox("HalfLam", &useHalfLam);
		ImGui::Checkbox("texture", &useTex);
		ImGui::ColorEdit4("color", &color.x);

		ImGui::EndMenu();
	}
}

void InstancingModel::Initialize(
	std::string name,
	int point,
	int instancingNum,
	ID3D12Resource* vertexRtea,
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView) {

	DXF_ = DirectXFunc::GetInstance();

	pso_->Initialize(DXF_->GetDevice());

	//各データ受け渡し
	point_ = point;
	instancingNum_ = instancingNum;
	vertexData_ = vertexRtea;
	vertexBufferView_ = vertexBufferView;
	
	//WVP用のリソースを作る。Matrix４ｘ４1つ分のサイズを用意する
	wvpResource_ = CreateBufferResource(DXF_->GetDevice(), sizeof(WorldTransformation) * instancingNum);
	//データを書き込む
	//書き込むためのアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
	//単位行列を書き込んでおくtextureResource
	for (uint32_t index = 0; index < (uint32_t)instancingNum; ++index) {
		wvpData_[index].WVP = MakeIdentity4x4();
		wvpData_[index].World = MakeIdentity4x4();
		wvpData_[index].color = { 1,1,1,1 };
	}

#pragma region マテリアル
	//マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource_ = CreateBufferResource(DXF_->GetDevice(), sizeof(Material));
	//マテリアルにデータを書き込む
	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = true;
	materialData_->uvTransform = MakeIdentity4x4();
	materialData_->enableTexture = true;
	materialData_->enableHalfLambert = true;
	materialData_->discardNum = 0.0f;
#pragma endregion

#pragma region ライト
	//ディレクションライトのマテリアルリソース
	directionalLightResource_ = CreateBufferResource(DXF_->GetDevice(), sizeof(DirectionalLight));
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	directionalLightData_->direction = Vector3(0.0f, -1.0f, 0.0f);
	directionalLightData_->intensity = 1.0f;
#pragma endregion

	SRVManager* SRVM = SRVManager::GetInstance();

#pragma region テクスチャ関係
	//スプライトの指定がない場合
	if (name == "") {
		int tex = TextureManager::uvChecker_;
		texture_ = SRVM->GetTextureDescriptorHandle(tex);
	}
	else {
		//指定があった場合
		int texture = TextureManager::LoadTex(name);
		texture_ = SRVM->GetTextureDescriptorHandle(texture);
	}
#pragma endregion
#pragma region instancing関係のデータ
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC instancingDesc{};
		instancingDesc.Format = DXGI_FORMAT_UNKNOWN;
		instancingDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		instancingDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		instancingDesc.Buffer.FirstElement = 0;
		instancingDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		instancingDesc.Buffer.NumElements = instancingNum;
		instancingDesc.Buffer.StructureByteStride = sizeof(WorldTransformation);

		SRVManager* SRVM = SRVManager::GetInstance();
		int instancingHandleNum = SRVM->CreateSRV(wvpResource_, nullptr, instancingDesc);
		instancingHandle_ = SRVM->GetTextureDescriptorHandle(instancingHandleNum);
	}
#pragma endregion

	Log("Model is Created!\n");
}
