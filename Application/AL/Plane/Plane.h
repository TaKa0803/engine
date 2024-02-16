#pragma once
#include "SingleGameObjects/GameObject.h"
#include"Camera/Camera.h"
class Plane :public GameObject {

public:

	Plane();

	//void CaluclateInit();

	void Initialize();

	void DebagWindow();

	void Draw(const Camera& viewp);


};

