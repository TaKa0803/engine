#include"SceneManager/SceneManager.h"

#include"./Scenes/Scenes.h"

#include<imgui.h>


#pragma region シーンのh
#include"./Scenes/TitleScene.h"
#include"./Scenes/GameScene.h"
#include"Scenes/ClearScene/ClearScene.h"
#include"Scenes/FailScene/FailScene.h"
#include"Scenes/DebugScene.h"
#pragma endregion



void SceneManager::Initialize()
{
	currentSceneNo_ = -1;
	//シーンの数取得
	sceneArr_.resize((size_t)SCENE::SceneCount);

	//各シーンの情報設定
	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[GAME] = std::make_unique<GameScene>();
	sceneArr_[CLEAR] = std::make_unique<ClearScene>();
	sceneArr_[FAIL] = std::make_unique<FailScene>();
	sceneArr_[DEBUG] = std::make_unique<DebugScene>();
	
	sceneName_.clear();
	sceneName_.push_back("TITLE");
	sceneName_.push_back("GAME");
	sceneName_.push_back("CLEAR");
	sceneName_.push_back("FAIL");
	sceneName_.push_back("DEBUG");
	


	/*
	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[STAGE] = std::make_unique<PlayScene>();
	sceneArr_[CLEAR] = std::make_unique<ClearScene>();
	*/


	//初期シーン設定
	IScene::SetSceneNo(TITLE);

}

void SceneManager::Update()
{
	
	//シーンチェック
	prevSceneNo_ = currentSceneNo_;
	currentSceneNo_ = IScene::GetSceneNo();
	
	//デバッグ表示
	DebugWindow();

	//シーン変更チェック
	if (prevSceneNo_ != currentSceneNo_)
	{
		//変更していたら		
		//初期化処理
		sceneArr_[currentSceneNo_]->Initialize();
	}

	
	//シーン更新処理
	sceneArr_[currentSceneNo_]->Update();


}

void SceneManager::Draw()
{
	//描画処理
	sceneArr_[currentSceneNo_]->Draw();

}

void SceneManager::EndFrame()
{

}

void SceneManager::Finalize()
{

}

void SceneManager::DebugWindow()
{
#ifdef _DEBUG

	int num = IScene::GetSceneNo();

	ImGui::Begin("SceneManager");
	ImGui::Text("SceneNo.%d", currentSceneNo_);
	ImGui::Text("%s", sceneName_[currentSceneNo_].c_str());
	ImGui::SliderInt("sceneNo", &num,TITLE, DEBUG);
	ImGui::End();

	IScene::SetSceneNo(num);
#endif // _DEBUG

}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager ins;
	return &ins;
}