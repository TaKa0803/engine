#include "Model.h"

#include"functions/function.h"
#include"TextureManager/TextureManager.h"
#include"Log/Log.h"

#include"ImGuiManager/ImGuiManager.h"
#include"SRVManager/SRVManager.h"

#include"struct.h"

#define _USE_MATH_DEFINES
#include<math.h>
#include<sstream>
#include<cassert>
#include<fstream>


#pragma region モデル
Model::~Model() {

	delete grarphics_;

	vertexData_->Release();
	wvpResource_->Release();
	materialResource_->Release();
	directionalLightResource_->Release();
	cameraResource_->Release();
}

Model* Model::CreateSphere(float kSubdivision,bool enableLighting, const std::string& filePath)
{
	DirectXFunc*DXF= DirectXFunc::GetInstance();

#pragma region 円
#pragma region VertexBufferViewを作成
	int point = (int)kSubdivision * (int)kSubdivision * 6;

	
	ID3D12Resource* vertexResourceSphere = CreateBufferResource(DXF->GetDevice(), sizeof(VertexData) * point);

	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSphere{};

	//リソースの戦闘のアドレスから使う
	vertexBufferViewSphere.BufferLocation = vertexResourceSphere->GetGPUVirtualAddress();
	//使用するリソースのサイズ
	vertexBufferViewSphere.SizeInBytes = sizeof(VertexData) * point;
	//1頂点当たりのサイズ
	vertexBufferViewSphere.StrideInBytes = sizeof(VertexData);
#pragma endregion
#pragma region Resourceにデータを書き込む
	//時計周りに点を設定していく
	VertexData* vertexDataS = nullptr;
	//書き込むためのアドレスを取得
	vertexResourceSphere->Map(0, nullptr,
		reinterpret_cast<void**>(&vertexDataS));

	//経度一つ分の角度δ
	const float kLonEvery = (float)M_PI * 2.0f / float(kSubdivision);
	//緯度一つ分の角度Θ
	const float kLatEvery = (float)M_PI / float(kSubdivision);
	//緯度の方向に分割
	for (int latIndex = 0; latIndex < kSubdivision; ++latIndex) {//σ
		float lat = -(float)M_PI / 2.0f + kLatEvery * latIndex;
		//経度のほう
		for (int lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {//Θ
			uint32_t start = ((int)latIndex * (int)kSubdivision + (int)lonIndex) * 6;
			float lon = lonIndex * kLonEvery;
			//一枚目
			//1(a)
			vertexDataS[start].position = { cos(lat) * cos(lon),sin(lat),  cos(lat) * sin(lon),1.0f };
			vertexDataS[start].texcoord = { (lonIndex) / (float)kSubdivision,1.0f - (latIndex) / (float)kSubdivision };
			vertexDataS[start].normal = { vertexDataS[start].position.x,vertexDataS[start].position.y,vertexDataS[start].position.z };

			//2(b)
			vertexDataS[start + 1].position = { cos(lat + kLatEvery) * cos(lon),sin(lat + kLatEvery), cos(lat + kLatEvery) * sin(lon),1.0f };
			vertexDataS[start + 1].texcoord = { (lonIndex) / (float)kSubdivision,1.0f - (latIndex + 1) / (float)kSubdivision };
			vertexDataS[start + 1].normal = { vertexDataS[start + 1].position.x,vertexDataS[start + 1].position.y,vertexDataS[start + 1].position.z };

			//3(c)
			vertexDataS[start + 2].position = { cos(lat) * cos(lon + kLonEvery),sin(lat),  cos(lat) * sin(lon + kLonEvery),1.0f };
			vertexDataS[start + 2].texcoord = { (lonIndex + 1) / (float)kSubdivision,1.0f - (latIndex) / (float)kSubdivision };
			vertexDataS[start + 2].normal = { vertexDataS[start + 2].position.x,vertexDataS[start + 2].position.y,vertexDataS[start + 2].position.z };

			//二枚目
			//
			vertexDataS[start + 3] = vertexDataS[start + 1];
			//
			vertexDataS[start + 4].position = { cos(lat + kLatEvery) * cos(lon + kLonEvery),sin(lat + kLatEvery), cos(lat + kLatEvery) * sin(lon + kLonEvery),1.0f };
			vertexDataS[start + 4].texcoord = { (lonIndex + 1) / (float)kSubdivision,1.0f - (latIndex + 1) / (float)kSubdivision };
			vertexDataS[start + 4].normal = { vertexDataS[start + 4].position.x,vertexDataS[start + 4].position.y,vertexDataS[start + 4].position.z };
			//
			vertexDataS[start + 5] = vertexDataS[start + 2];
		}
	}
#pragma endregion
#pragma region wvp設定
	ID3D12Resource* wvpResourceS;

	//WVP用のリソースを作る。Matrix４ｘ４1つ分のサイズを用意する
	wvpResourceS = CreateBufferResource(DXF->GetDevice(), sizeof(WorldTransformation));
	//データを書き込む
	WorldTransformation* wvpDataS = nullptr;

	//書き込むためのアドレスを取得
	wvpResourceS->Map(0, nullptr, reinterpret_cast<void**>(&wvpDataS));
	//単位行列を書き込んでおくtextureResource
	wvpDataS->WVP = MakeIdentity4x4();
	wvpDataS->World = MakeIdentity4x4();
#pragma endregion	
#pragma endregion	




	Model* model = new Model();
	model->Initialize(filePath, point,vertexResourceSphere, vertexBufferViewSphere, wvpResourceS, wvpDataS);
	
	
	
	return model;

}

Model* Model::CreateFromOBJ(const std::string& filePath)
{
	DirectXFunc* DXF = DirectXFunc::GetInstance();

#pragma region モデル
	ModelManager*mManager= ModelManager::GetInstance();
	

	ModelData modeltea =mManager->GetModelData(filePath); 

	
	ID3D12Resource* vertexRtea = CreateBufferResource(DXF->GetDevice(), sizeof(VertexData) * modeltea.vertices.size());
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewtea{};
	vertexBufferViewtea.BufferLocation = vertexRtea->GetGPUVirtualAddress();
	vertexBufferViewtea.SizeInBytes = UINT(sizeof(VertexData) * modeltea.vertices.size());
	vertexBufferViewtea.StrideInBytes = sizeof(VertexData);

	VertexData* vertexDatatea = nullptr;
	vertexRtea->Map(0, nullptr, reinterpret_cast<void**>(&vertexDatatea));
	std::memcpy(vertexDatatea, modeltea.vertices.data(), sizeof(VertexData) * modeltea.vertices.size());


	//WVP用のリソースを作る。Matrix４ｘ４1つ分のサイズを用意する
	ID3D12Resource* wvpResourceTea = CreateBufferResource(DXF->GetDevice(), sizeof(WorldTransformation));
	//データを書き込む
	WorldTransformation* wvpDataTea = nullptr;
	//書き込むためのアドレスを取得
	wvpResourceTea->Map(0, nullptr, reinterpret_cast<void**>(&wvpDataTea));
	//単位行列を書き込んでおくtextureResource
	wvpDataTea->WVP = MakeIdentity4x4();
	wvpDataTea->World = MakeIdentity4x4();
#pragma endregion



	Model* model =new Model();
	model->Initialize(modeltea.material.textureFilePath,UINT(modeltea.vertices.size()),vertexRtea, vertexBufferViewtea, wvpResourceTea, wvpDataTea);


	
	
	return model;
}

void Model::Initialize(
	std::string name_,
	int point,
	ID3D12Resource* vertexRtea,
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView,
	ID3D12Resource* wvpResource,
	WorldTransformation* wvpData
) {

	DXF_ = DirectXFunc::GetInstance();

	grarphics_ = new ObjectPSO();
	grarphics_->Initialize(DXF_->GetDevice());

	name = name_;


	SRVManager* SRVM = SRVManager::GetInstance();

	//スプライトの指定がない場合
	if (name_ == "") {
		int tex = TextureManager::uvChecker_;
		texture_ = SRVM->GetTextureDescriptorHandle(tex);
	}
	else {
		//指定があった場合
		int texture = TextureManager::LoadTex(name);	
		texture_ = SRVM->GetTextureDescriptorHandle(texture);
	}

	point_ = point;
	vertexData_ = vertexRtea;
	vertexBufferView_ = vertexBufferView;
	wvpData_ = wvpData;
	wvpResource_ = wvpResource;
	

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
	materialData_->enablePhongReflection = 0;
	materialData_->shininess = 20.0f;
#pragma endregion

#pragma region ライト
	//ディレクションライトのマテリアルリソース
	directionalLightResource_ = CreateBufferResource(DXF_->GetDevice(), sizeof(DirectionalLight));
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	directionalLightData_->direction = Vector3(0.0f, -1.0f, 0.0f);
	directionalLightData_->intensity = 1.0f;
#pragma endregion

#pragma region カメラ関係
	cameraResource_ = CreateBufferResource(DXF_->GetDevice(), sizeof(Camera4GPU));
	cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));
	cameraData_->worldPosition = { 0,0,0 };
#pragma endregion


	Log("Model is Created!\n");
}


void Model::Draw(const Matrix4x4& worldMatrix, const Camera& camera,int texture)
{
	grarphics_->PreDraw(DXF_->GetCMDList());

	materialData_->uvTransform = MakeAffineMatrix(uvscale, uvrotate, uvpos);

	Matrix4x4 WVP = worldMatrix* camera.GetViewProjectionMatrix();

	wvpData_->WVP = WVP;
	wvpData_->World = worldMatrix;

	cameraData_->worldPosition = camera.GetMainCamera().GetMatWorldTranslate();

	DXF_->GetCMDList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//形状を設定、PSOに設定しているものとはまた別、同じものを設定すると考えておけばいい
	DXF_->GetCMDList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//wvp用のCBufferの場所の設定
	DXF_->GetCMDList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	//マテリアルCBufferの場所を設定
	DXF_->GetCMDList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//
	DXF_->GetCMDList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());
	//カメラ位置転送
	DXF_->GetCMDList()->SetGraphicsRootConstantBufferView(4, cameraResource_->GetGPUVirtualAddress());
	if (texture == -1) {
		DXF_->GetCMDList()->SetGraphicsRootDescriptorTable(2, texture_);
	}
	else {
		//SRVのDescriptorTableの先頭を設定。２はParameter[2]である。
		DXF_->GetCMDList()->SetGraphicsRootDescriptorTable(2, SRVManager::GetInstance()->GetTextureDescriptorHandle(texture));
	}
	//描画！		
	DXF_->GetCMDList()->DrawInstanced(point_, 1, 0, 0);
}

void Model::DebugParameter(const char* name)
{
#ifdef _DEBUG
	bool useTexture = materialData_->enableTexture;
	bool useShader = materialData_->enableLighting;
	bool useHalf = materialData_->enableHalfLambert;
	Vector4 color = materialData_->color;

	BlendMode blend = grarphics_->GetBlendMode();
	const char* items[] = { "None","Normal","Add","Subtract","Multiply","Screen" };
	int currentItem = static_cast<int>(blend);

	float discardnum = materialData_->discardNum;

	bool usePhong = materialData_->enablePhongReflection;
	float shininess = materialData_->shininess;


	if (ImGui::BeginMenu(name)) {
		ImGui::Checkbox("Texture", &useTexture);
		ImGui::Checkbox("Shader", &useShader);
		ImGui::Checkbox("HalfLambert", &useHalf);
		ImGui::ColorEdit4("color", &color.x);
		if (ImGui::Combo("blendmode", &currentItem, items, IM_ARRAYSIZE(items))) {
			blend = static_cast<BlendMode>(currentItem);
		}
		ImGui::DragFloat("discardNum", &discardnum, 0.01f);

			ImGui::Text("UV");
			ImGui::DragFloat2("uv pos", &uvpos.x, 0.1f);
		ImGui::DragFloat("uv rotate", &uvrotate.z, 0.1f);
		ImGui::DragFloat2("uv scale", &uvscale.x, 0.1f);

		ImGui::Text("color");
		ImGui::DragFloat3("light direction", &directionalLightData_->direction.x, 0.01f);
		ImGui::DragFloat("light intensity", &directionalLightData_->intensity, 0.01f);
		ImGui::ColorEdit4("light color", &directionalLightData_->color.x);
		ImGui::Checkbox("PhongReflection", &usePhong);
		ImGui::DragFloat("Shininess", &shininess);
		ImGui::EndMenu();
	}
	

	materialData_->enableTexture = useTexture;
	materialData_->enableLighting = useShader;
	materialData_->enableHalfLambert = useHalf;
	materialData_->color = color;
	grarphics_->SetBlendMode(blend);
	materialData_->discardNum= discardnum;
	materialData_->enablePhongReflection = usePhong;
	materialData_->shininess = shininess;
#endif // _DEBUG
	
}
#pragma endregion


