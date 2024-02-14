#include"Sprite.h"
#include"Math/Matrix.h"
#include<cassert>
#include"TextureManager/TextureManager.h"
#include"SRVManager/SRVManager.h"
#include"functions/function.h"
#include<imgui.h>
#include"Log/Log.h"


Sprite::Sprite() {
}

Sprite::~Sprite() {
	delete grarphics_;
	vertexResource_->Release();
	indexResource_->Release();
	transformationMatrixResource_->Release();
	materialResource_->Release();
}

void Sprite::DrawDebugImGui(const char* name) {

	
#ifdef _DEBUG
	Vector4 color = materialData_->color;
	if (ImGui::BeginMenu(name)) {
		ImGui::DragFloat3("pos", &world_.translate_.x, 0.1f);
		ImGui::DragFloat("rotate", &world_.rotate_.z);
		ImGui::DragFloat2("scale", &world_.scale_.x, 0.1f);

		ImGui::Text("UV");
		ImGui::DragFloat2("uv pos", &uvWorld_.translate_.x, 0.1f);
		ImGui::DragFloat("uv rotate", &uvWorld_.rotate_.z, 0.1f);
		ImGui::DragFloat2("uv scale", &uvWorld_.scale_.x, 0.1f);



		BlendMode blend = grarphics_->GetBlendMode();
		const char* items[] = { "None","Normal","Add","Subtract","Multiply","Screen" };
		int currentItem = static_cast<int>(blend);

		if (ImGui::Combo("blendmode", &currentItem, items, IM_ARRAYSIZE(items))) {
			blend = static_cast<BlendMode>(currentItem);
		}

		grarphics_->SetBlendMode(blend);


		ImGui::EndMenu();
	}
#endif // _DEBUG

}




Sprite* Sprite::Create(int texture, const Vector2 size, const Vector2 Rect, const Vector2 scale , const Vector2 translate, const Vector2 anchor, const float rotate) {
	DirectXFunc* DXF = DirectXFunc::GetInstance();
	
	WorldTransform newWorld;
	newWorld.translate_ = { translate.x,translate.y,0 };
	newWorld.rotate_.z = rotate;
	newWorld.scale_ = { scale.x,scale.y,1 };
#pragma region Sprite
	ID3D12Resource* vertexResource;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	ID3D12Resource* indexResourceSprite;

	D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite{};


#pragma region VertexResourceとVertexBufferViewを用意
	//Sprite用の頂点リソースを作る
	vertexResource = CreateBufferResource(DXF->GetDevice(), sizeof(VertexData) * 4);

	//頂点バッファビューを作成する
	//リソース用の先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 4;
	//頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);
#pragma endregion

#pragma region 頂点データを設定する
	VertexData* vertexDataSprite = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite));
	//一枚目の三角形

	Vector2 minv = { 1 * (-anchor.x),1 * (-anchor.y) };

	Vector2 maxV = { 1 * (1 - anchor.x),1 * (1 - anchor.y) };

	Vector2 maxUV = { Rect.x / size.x,Rect.y/size.y };

	vertexDataSprite[0].position = { minv.x,maxV.y,0.0f,1.0f };
	vertexDataSprite[0].texcoord = { 0.0f,maxUV.y };
	vertexDataSprite[0].normal = { 0.0f,0.0f,-1.0f };

	vertexDataSprite[1].position = { minv.x,minv.y,0.0f,1.0f };
	vertexDataSprite[1].texcoord = { 0.0f,0.0f };
	vertexDataSprite[1].normal = { 0.0f,0.0f,-1.0f };

	vertexDataSprite[2].position = { maxV.x,maxV.y,0.0f,1.0f };
	vertexDataSprite[2].texcoord = maxUV;
	vertexDataSprite[2].normal = { 0.0f,0.0f,-1.0f };

	vertexDataSprite[3].position = { maxV.x,minv.y,0.0f,1.0f };
	vertexDataSprite[3].texcoord = { maxUV.x,0.0f };
	vertexDataSprite[3].normal = { 0.0f,0.0f,-1.0f };

	indexResourceSprite = CreateBufferResource(DXF->GetDevice(), sizeof(uint32_t) * 6);

	//リソースの先頭アドレスから使う
	indexBufferViewSprite.BufferLocation = indexResourceSprite->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス６つ分のサイズ
	indexBufferViewSprite.SizeInBytes = sizeof(uint32_t) * 6;
	//インデックス
	indexBufferViewSprite.Format = DXGI_FORMAT_R32_UINT;

	uint32_t* indexDataSprite = nullptr;
	indexResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSprite));
	indexDataSprite[0] = 1;
	indexDataSprite[1] = 3;
	indexDataSprite[2] = 0;

	indexDataSprite[3] = 3;
	indexDataSprite[4] = 2;
	indexDataSprite[5] = 0;


#pragma endregion



	Sprite* sprite = new Sprite();
	sprite->Initialize(texture,newWorld, vertexResource, vertexBufferView, indexResourceSprite, indexBufferViewSprite);

	return sprite;
}


/*
Sprite* Sprite::Create(int texture, const Vector2 translate, const float rotate, const Vector2 scale, const Vector2 anchor) {

	DirectXFunc* DXF = DirectXFunc::GetInstance();




	WorldTransform newWorld;
	newWorld.translate_ = { translate.x,translate.y,1 };
	newWorld.rotate_.z = rotate;
	newWorld.scale_ = { scale.x,scale.y,1 };

#pragma region Sprite
	ID3D12Resource* vertexResource;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	ID3D12Resource* indexResourceSprite;

	D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite{};


#pragma region VertexResourceとVertexBufferViewを用意
	//Sprite用の頂点リソースを作る
	vertexResource = CreateBufferResource(DXF->GetDevice(), sizeof(VertexData) * 4);

	//頂点バッファビューを作成する
	//リソース用の先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 4;
	//頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);
#pragma endregion

#pragma region 頂点データを設定する
	VertexData* vertexDataSprite = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite));
	//一枚目の三角形

	Vector2 minv = { 1 * (-anchor.x),1 * (-anchor.y) };

	Vector3 maxV = { 1 * (1 - anchor.x),1 * (1 - anchor.y) };

	vertexDataSprite[0].position = { minv.x,maxV.y,0.0f,1.0f };
	vertexDataSprite[0].texcoord = { 0.0f,1.0f };
	vertexDataSprite[0].normal = { 0.0f,0.0f,-1.0f };

	vertexDataSprite[1].position = { minv.x,minv.y,0.0f,1.0f };
	vertexDataSprite[1].texcoord = { 0.0f,0.0f };
	vertexDataSprite[1].normal = { 0.0f,0.0f,-1.0f };

	vertexDataSprite[2].position = { maxV.x,maxV.y,0.0f,1.0f };
	vertexDataSprite[2].texcoord = { 1.0f,1.0f };
	vertexDataSprite[2].normal = { 0.0f,0.0f,-1.0f };

	vertexDataSprite[3].position = { maxV.x,minv.y,0.0f,1.0f };
	vertexDataSprite[3].texcoord = { 1.0f,0.0f };
	vertexDataSprite[3].normal = { 0.0f,0.0f,-1.0f };

	indexResourceSprite = CreateBufferResource(DXF->GetDevice(), sizeof(uint32_t) * 6);

	//リソースの先頭アドレスから使う
	indexBufferViewSprite.BufferLocation = indexResourceSprite->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス６つ分のサイズ
	indexBufferViewSprite.SizeInBytes = sizeof(uint32_t) * 6;
	//インデックス
	indexBufferViewSprite.Format = DXGI_FORMAT_R32_UINT;

	uint32_t* indexDataSprite = nullptr;
	indexResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSprite));
	indexDataSprite[0] = 1;
	indexDataSprite[1] = 3;
	indexDataSprite[2] = 0;

	indexDataSprite[3] = 3;
	indexDataSprite[4] = 2;
	indexDataSprite[5] = 0;


#pragma endregion



	Sprite* sprite = new Sprite();
	sprite->Initialize(texture,newWorld, vertexResource, vertexBufferView, indexResourceSprite, indexBufferViewSprite);

	return sprite;

}
*/


void Sprite::Initialize(int texture,

	WorldTransform world,

	ID3D12Resource* vertexResource,

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView,

	ID3D12Resource* indexResourceSprite,

	D3D12_INDEX_BUFFER_VIEW indexBufferView
) {
	DXF = DirectXFunc::GetInstance();

	grarphics_ = new ObjectPSO();
	grarphics_->Initialize(DXF->GetDevice());


	//データコピー

	texture_ = texture;

	world_ = world;

	vertexResource_ = vertexResource;

	vertexBufferView_ = vertexBufferView;

	indexResource_ = indexResourceSprite;

	indexBufferView_ = indexBufferView;




#pragma region Transform周りを作る
	transformationMatrixResource_ = CreateBufferResource(DXF->GetDevice(), sizeof(WorldTransformation));
	//データを書き込む

	//書き込むためのアドレスを取得
	transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	transformationMatrixData_->WVP = MakeIdentity4x4();
	transformationMatrixData_->World = MakeIdentity4x4();
#pragma endregion

	//Sprite用のマテリアルリソース
	materialResource_ = CreateBufferResource(DXF->GetDevice(), sizeof(Material));
	//マテリアルにデータを書き込む

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = false;
	materialData_->uvTransform = MakeIdentity4x4();
	materialData_->enableTexture = true;
	materialData_->enableHalfLambert = false;
#pragma endregion


	Log("Sprite is Created!\n");
}


void Sprite::Draw(int texture) {

	//PSO
	grarphics_->PreDraw(DXF->GetCMDList());

	//uv更新
	uvWorld_.UpdateMatrix();
	//uvTransform更新
	materialData_->uvTransform = uvWorld_.matWorld_;

	//ワールド更新
	world_.UpdateMatrix();
	Matrix4x4 World = world_.matWorld_;

	//スプライト用データ
	Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(WindowApp::kClientWidth), float(WindowApp::kClientHeight), 0.0f, 100.0f);
	Matrix4x4 VPSprite = viewMatrixSprite * projectionMatrixSprite;
	Matrix4x4 WVP = World * VPSprite;
	//データ代入
	transformationMatrixData_->WVP = WVP;
	transformationMatrixData_->World = World;

	//Spriteの描画
	DXF->GetCMDList()->IASetVertexBuffers(0, 1, &vertexBufferView_);	//VBVを設定			
	DXF->GetCMDList()->IASetIndexBuffer(&indexBufferView_);//IBVを設定
	//形状を設定、PSOに設定しているものとはまた別、同じものを設定すると考えておけばいい
	DXF->GetCMDList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//マテリアルCBufferの場所を設定
	DXF->GetCMDList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//TransformationMatrixCBufferの場所を設定
	DXF->GetCMDList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
	//
	if (texture != -1) {
		DXF->GetCMDList()->SetGraphicsRootDescriptorTable(2, SRVManager::GetInstance()->GetTextureDescriptorHandle(texture));
	}
	else {
		DXF->GetCMDList()->SetGraphicsRootDescriptorTable(2, SRVManager::GetInstance()->GetTextureDescriptorHandle(texture_));
	}
	//描画！！（DrawCall
	DXF->GetCMDList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}



