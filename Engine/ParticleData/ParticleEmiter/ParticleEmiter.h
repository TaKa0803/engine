#pragma once
#include"Math/Vector2.h"
#include"Particle/Particle.h"
#include"DirectXFunc/DirectXFunc.h"
//#include"SingleGameObjects/GameObject.h"
#include"struct.h"
#include"WorldTransform/WorldTransform.h"
#include"Camera/Camera.h"
#include"SingleGameObjects/ObjectPSO.h"

#include<vector>

class ParticleEmiter {

public:
	ParticleEmiter();
	~ParticleEmiter();

	
	/// <summary>
	/// インスタンシングでデータを生成
	/// </summary>
	/// <param name="texture">画像</param>
	/// <param name="occurrenceMaxCount">描画の最大数</param>
	/// <param name="size">scaleが１の時のサイズ</param>
	/// <param name="spriteSize">画像のサイズ</param>
	/// <param name="Rect">画像をどの範囲描画するか</param>
	/// <param name="anchor">スプライトの中心点</param>
	/// <returns>設定を行ったスプライトデータ</returns>
	static  ParticleEmiter* Create2D(const Camera*camera, int texture,const int occurrenceMaxCount , const Vector2 spriteSize,const Vector2 Rect,const Vector2 anchor={0.5f,0.5f});

	/// <summary>
	/// インスタンシングで3Dトデータを生成
	/// </summary>
	/// <param name="texture">画像</param>
	/// <param name="occurrenceMaxCount">描画の最大数</param>
	/// <param name="size">scaleが１の時のサイズ</param>
	/// <param name="spriteSize">画像のサイズ</param>
	/// <param name="Rect">画像をどの範囲描画するか</param>
	/// <param name="anchor">スプライトの中心点</param>
	/// <returns>設定を行ったスプライトデータ</returns>
	static  ParticleEmiter* Create3D(const Camera* camera, int texture, const int occurrenceMaxCount, const Vector2 spriteSize, const Vector2 Rect, const Vector2 anchor = { 0.5f,0.5f });

	void Update();

	//スプライト
	void SetParticle(Particle *particle){ particles_.push_back(particle); }

	void Draw2D(int texture = -1);

	void Draw3D(int texture = -1);

	void DebugImGui(const char* name);
private:

	void Initialize(const Camera* camera, int32_t texture,int32_t occurrenceMaxCount, ID3D12Resource* vertexResource, D3D12_VERTEX_BUFFER_VIEW vertexBufferView,ID3D12Resource* indexResource, D3D12_INDEX_BUFFER_VIEW indexBufferView,bool is2D=false);

private:


	bool is2D_=false;

	DirectXFunc* DXF_ = nullptr;

	ParticleGraphics* particlegraphics_ = nullptr;

	const Camera* camera_ = nullptr;

	int texture_ = -1;

	int instancingHandleNum = -1;

	static int instancingCount_;

	ID3D12Resource* vertexResource_ = nullptr;
	ID3D12Resource* indexResource_ = nullptr;
	//
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	//
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};
	//
	ID3D12Resource* transformationMatrixResource_ = nullptr;;
	//
	
	Particle4GPU* particle4GPUData_ = nullptr;

	Material* materialData_ = nullptr;;
	
	//
	ID3D12Resource* materialResource_ = nullptr;


	Matrix4x4 viewMatrixSprite = MakeIdentity4x4();

	std::vector<Particle*>particles_;

	
	WorldTransform uvWorld_;

	Vector3 scale_ = { 1,1,1 };
	Vector3 rotate_ = { 0,0,0 };
};