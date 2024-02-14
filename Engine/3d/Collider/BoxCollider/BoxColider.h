#pragma once
#include"WorldTransform/WorldTransform.h"
#include"SingleGameObjects/Model.h"
#include"IsCollisions.h"

enum class ColliderType {
	Sphere,
	Box
};

class Collider {
protected:
	Model* model_;
	int texture_;


public:
	WorldTransform world_;

	ColliderType type;

	AABB aabb;

public:
	~Collider();

	void Initialize(const WorldTransform* world = nullptr);

	void Update();

	void Draw(const Matrix4x4& VP);
	
	const WorldTransform& GetWorld() { return world_; }

	
	
};


class BoxColider: private Collider {
public:

	~BoxColider() ;

	void Initialize(const WorldTransform* world);
	
	void Update();

	void Draw(const Matrix4x4& VP);

	
	void DebugImGui(const char* name);
	
	const AABB& GetAABB()const { return aabb; }

	//当たっていたら押し戻し処理
	void Collision(const Collider& collider,Vector3&pushBackVelo );



public://ゲッター
#pragma region ゲッター
	void SetTranslate(const Vector3& pos) { world_.translate_ = pos; }
	void SetScale(const Vector3& scale) { world_.scale_ = scale; }
#pragma endregion


public://セッター


private:
	
};

