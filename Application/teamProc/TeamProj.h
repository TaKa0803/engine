#pragma once
#include "IScene/IScene.h"

#include"Camera/Camera.h"
#include"teamProc/teamP.h"

#include<algorithm>

class TeamProj :public IScene
{
public:

	TeamProj();
	~TeamProj();

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:

	void Debug();

private:

	std::unique_ptr<TeamPlayer>player_;

	
	std::unique_ptr<Camera>camera_;

};

