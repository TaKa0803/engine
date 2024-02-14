#include "Stage.h"
#include <imgui.h>

using Placement = StageWall::Placement;

Stage::Stage()
{
	for (size_t i = 0; i < Placement::_COUNT; i++)
	{
		walls_.emplace_back(new StageWall);
	}
}

void Stage::Initialize()
{
	for (size_t i = 0; i < Placement::_COUNT; i++)
	{
		walls_[i]->Initialize(i);
	}
}

void Stage::Update()
{
	for (size_t i = 0; i < Placement::_COUNT; i++)
	{
		walls_[i]->Update();
	}
}

void Stage::Draw()
{
	for (size_t i = 0; i < Placement::_COUNT; i++)
	{
		walls_[i]->Draw();
	}
}
#include <numbers>

void Stage::DebagWindow()
{

#ifdef _DEBUG


	ImGui::Begin("Stage");

	ImGui::Text("Walls:%d", walls_.size());
	ImGui::Separator();

	for (size_t i = 0; i < Placement::_COUNT; i++)
	{
		std::string tree = "wall:" + std::to_string(i);
		if (ImGui::TreeNode(tree.c_str()))
		{
			walls_[i]->DebagWindow();

			ImGui::TreePop();
		}
		ImGui::Separator();
	}

	for (size_t i = 0; i < Placement::_COUNT; i++)
	{
		walls_[i]->Load();
	}
	if (ImGui::Button("SaveAllData"))
	{
		for (size_t i = 0; i < Placement::_COUNT; i++)
		{
			walls_[i]->SaveGlobalVariable();
		}
	}

	ImGui::Separator();

	for (size_t i = 0; i < Placement::_COUNT; i++)
	{
		float pi = static_cast<float>(std::numbers::pi);
		float theta = (2 * pi) / (float)Placement::_COUNT;
		theta *= i;
		Vector3 pos = { std::cosf(theta),0.0f,std::sinf(theta) };
		ImGui::Text("pos : %.2f, %.2f, %.2f", pos.x, pos.y, pos.z);
		ImGui::Text("%d  : %f", i, theta);
		ImGui::Separator();
	}

	ImGui::End();

#endif // _DEBUG

}