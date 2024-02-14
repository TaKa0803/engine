#include "MainSystem.h"
#include"SceneManager/SceneManager.h"

#include"GlobalVariables/GlobalVariables.h"
#include"RandomNum/RandomNum.h"
#include"AudioManager/AudioManager.h"

#include"SingleGameObjects/ObjectPSO.h"
#include"DXC/DXCManager.h"

MainSystem* MainSystem::GetInstance() {
	static MainSystem instance;
	return &instance;
}

void MainSystem::Run() {
	HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);

	//エンジン初期化処理
	Initializes();

	//更新処理
	MainRoop();

	//終了処理
	Finalize();

	//終わり
	CoUninitialize();
}

void MainSystem::Initializes() {
	//windowsアプリケーション
	winApp_ = WindowApp::GetInstance();
	winApp_->Initialize(L"LE2A_07_キクチ",1280,720);

	//DirectX
	DXF_ = DirectXFunc::GetInstance();
	DXF_->Initialize(winApp_);

	//DXCマネージャ
	DXCManager*DXC= DXCManager::GetInstance();
	DXC->Initialize();

	//SRV
	SRVM_ = SRVManager::GetInstance();
	SRVM_->Initialize(DXF_);
	
	//画像関係
	textureManager_= TextureManager::GetInstance();
	textureManager_->Initialize(DXF_);

	
	//imgui
	imguiManager_ = ImGuiManager::GetInstance();
	imguiManager_->Initialize(winApp_, DXF_);

	//入力
	input_ = Input::GetInstance();
	input_->Initialize(winApp_);
	
	//インスタンシングモデル
	instancingMM_ = InstancingModelManager::GetInstance();

	//乱数クラス
	randomNumClass_ = RandomNumber::GetInstance();
	randomNumClass_->RandomNumberProcessInitialize();
	
	//音声マネージャ
	AudioManager *audioManager = AudioManager::GetInstance();
	audioManager->Initialize();
	audioManager->LoadAllSoundData();

	
}

void MainSystem::MainRoop() {
	//保存データ取得
	GlobalVariables::GetInstance()->LoadFiles();

	//モデルデータ関係読み込み
	ModelManager::GetInstance()->LoadAllModels();
	instancingMM_->LoadAllModel();

	//音声データ読み込み
	
	//シーンマネージャ初期化
	SceneManager* sceneManager = SceneManager::GetInstance();
	sceneManager->Initialize();

	//読み込んだ画像をGPUに送信
	SRVM_->PostInitialize();

	while (winApp_->ProcessMessage()) {
		
#pragma region 状態更新
		///更新前処理
		//ImGui
		imguiManager_->PreUpdate();

		//インスタンシングの更新前処理
		instancingMM_->PreUpdate();

		//キー入力
		input_->Update();
		///=以下更新=//

#ifdef _DEBUG
		//GlobalVariableデータの更新処理
		GlobalVariables::GetInstance()->Update();
		auto delta = ImGui::GetIO().Framerate;

		ImGui::Begin("Engine");
		ImGui::Text("Frame %4.1f", delta);
		ImGui::End();
#endif // _DEBUG

		sceneManager->Update();

		//==更新終わり==//
		// 
		//更新終わり描画前処理
		imguiManager_->PostUpdate();
#pragma endregion
#pragma region 描画		
		///描画前処理
		//DirectX
		DXF_->PreDraw();
		//ImGui
		imguiManager_->PreDraw();
		
		//==以下描画==//
		sceneManager->Draw();
		
		//==描画終わり==//

		///描画あと処理
		//imGui
		imguiManager_->PostDraw();
		//DirectX
		DXF_->PostDraw();
#pragma endregion
		///フレーム終了時処理
		sceneManager->EndFrame();

		//シーン終了フラグがtrueの時
		if (IScene::GetLeaveScene()) {
			//シーンを終わる
			break;
		}
	}

	sceneManager->Finalize();
	//dScene->Finalize();
	//mt4->Finalize();

}


void MainSystem::Finalize() {
	///開放処理
	instancingMM_->Finalize();
	SRVM_->Finalize();
	textureManager_->Finalize();
	imguiManager_->Finalize();
	DXF_->Finalize();
	winApp_->Finalize();

}
