#pragma once

#include "StageWall.h"

class Stage
{
private:
	
	std::vector<std::unique_ptr<StageWall>> walls_;

public:
	Stage();

	// 値の初期化
	void Initialize();

	void Update();

	void Draw();

	void DebagWindow();

	auto& GetWalls() { return walls_; }
};
