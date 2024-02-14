#include"ImGuiManager.h"

#pragma region ImGui
#include<imgui_impl_dx12.h>
#include<imgui_impl_win32.h>
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#pragma endregion


#include<cassert>

ImGuiManager* ImGuiManager::GetInstance()
{
	static ImGuiManager Instance;
	return &Instance;
}

void ImGuiManager::Initialize(WindowApp* winApp,DirectXFunc *DXF)
{
	assert(winApp);
	winApp_ = winApp;

	assert(DXF);
	DXF_ = DXF;

	SRVM_ = SRVManager::GetInstance();
	
#pragma region ImGuiの初期化
	//ImGuiの初期化。詳細はさして重要ではないので解説は省略
	//こういうもんである
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winApp_->GetHwnd());
	ImGui_ImplDX12_Init(DXF_->GetDevice(),
		DXF_->GetswapChainDesc().BufferCount,
		DXF_->GetrtvDesc().Format,
		SRVM_->GetSRV(),
		SRVM_->GetSRV()->GetCPUDescriptorHandleForHeapStart(),
		SRVM_->GetSRV()->GetGPUDescriptorHandleForHeapStart());
#pragma endregion

}

void ImGuiManager::PreUpdate()
{
#pragma region フレームの先頭でImGuiにここからフレームが始まる旨を告げる	
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#pragma endregion
}

void ImGuiManager::PostUpdate()
{
	//ImGuiの内部コマンドを生成する
	ImGui::Render();
}

void ImGuiManager::PreDraw()
{
#pragma region ImGuiの処理
	//描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { SRVM_->GetSRV() };
	DXF_->GetCMDList()->SetDescriptorHeaps(1, descriptorHeaps);
#pragma endregion
}

void ImGuiManager::PostDraw()
{
	//実際のcommandListのImGuiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DXF_->GetCMDList());
}



void ImGuiManager::Finalize()
{
	//ImGui開放
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

}
