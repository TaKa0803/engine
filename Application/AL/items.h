#pragma once
#include"WorldTransform/WorldTransform.h"


//モデルの数
static const int modelNum_ = 5;


struct ModelAnimeParts {
	int RoopFrame;
	WorldTransform stPartsWorlds[modelNum_];
	WorldTransform edPartsWorlds[modelNum_];
};
