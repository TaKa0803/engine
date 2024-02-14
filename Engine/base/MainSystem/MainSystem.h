#pragma once

#include"WinApp/WinApp.h"
#include"DirectXFunc/DirectXFunc.h"
#include"InstancingModelManager/InstancingModelManager.h"
#include"TextureManager/TextureManager.h"
#include"SingleModelManager/ModelManager.h"
#include"ImGuiManager/ImGuiManager.h"
#include"Input/Input.h"
#include"SRVManager/SRVManager.h"
#include"RandomNum/RandomNum.h"



class MainSystem {
public://シングルトンパターン
	static MainSystem* GetInstance();

private://シングルトンパターン

	MainSystem() = default;
	~MainSystem() = default;
	MainSystem(const MainSystem& o) = delete;
	const MainSystem& operator=(const MainSystem& o) = delete;

public:

	void Run();

private:
	void Initializes();

	void MainRoop();

	void Finalize();

private:
#pragma region 基板初期化
	//windowsアプリケーション
	WindowApp* winApp_;

	//DirectX
	DirectXFunc* DXF_;

	//ImGui
	ImGuiManager* imguiManager_;

	//texture
	TextureManager* textureManager_;

	//入力
	Input* input_;

	//SRV
	SRVManager* SRVM_;

	//インスタンシングモデル
	InstancingModelManager* instancingMM_;

	//乱数クラス
	RandomNumber* randomNumClass_;
#pragma endregion
};