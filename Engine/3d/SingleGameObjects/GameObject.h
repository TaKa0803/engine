#pragma once

#include"SingleGameObjects/Model.h"
#include"WorldTransform/WorldTransform.h"
#include"Camera/Camera.h"
#include<vector>

class GameObject {

protected:

	Model* model_=nullptr;

	WorldTransform world_;

	
public:
	const WorldTransform& GetWorld() { return world_; }

	virtual ~GameObject();

	virtual void Initialize(const std::string&filepath );

	virtual void Update();

	virtual void Draw(const Camera&camera, Vector3 pointlight = { 0,0,0 },int tex =-1);

	virtual void Debug(const char*name);
};

