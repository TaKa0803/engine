#pragma once

#include"SingleGameObjects/GameObject.h"

class TeamPlayer : public GameObject {

public:

	void Initialize();

	void Update();

	void Draw(const Camera&camera);

private:

	Vector3 moveVelo = { 0,1,0 };


};