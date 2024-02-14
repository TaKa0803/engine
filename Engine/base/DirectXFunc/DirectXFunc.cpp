#include"DirectXFunc.h"
#include"Log/Log.h"
#include"functions/function.h"
#include"SRVManager/SRVManager.h"
#include<thread>
#include<cassert>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"winmm.lib")

DirectXFunc* DirectXFunc::GetInstance()
{	
	static DirectXFunc Instance;
	return &Instance;
}

#pragma region Initializeまとめ

void DirectXFunc::Initialize(WindowApp* winApp)
{
	assert(winApp);
	winApp_ = winApp;

#pragma region DebugLayer

//#ifdef _DEBUG
//
//	ID3D12Debug1* debugController = nullptr;
//	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
//		//デバッグレイヤーを有効化
//		debugController->EnableDebugLayer();
//		//さらにGPU側でもチェックを行うようにする
//		debugController->SetEnableGPUBasedValidation(TRUE);
//	}
//#endif

#pragma endregion

	D3D12Devicenitialize();
	CommandInitialize();
	SwapChainInitialize();
	RTVInitialize();
	DSVInitialize();
	FenceInitialize();

	FixFPSInitialize();

	Log("Complete DirectXFunc Initialize\n");
}

void DirectXFunc::FixFPSInitialize() {
	reference_ = std::chrono::steady_clock::now();

	timeBeginPeriod(1);
}

void DirectXFunc::UpdateFixFPS() {
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));

	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	if (elapsed < kMinCheckTime) {
		while (std::chrono::steady_clock::now()-reference_<kMinTime) {
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
	
	reference_ = std::chrono::steady_clock::now();

}

void DirectXFunc::D3D12Devicenitialize()
{
#pragma region DXGIFactoryの生成
	//DXGIファクトリーの生成
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));

	assert(SUCCEEDED(hr));
#pragma endregion
#pragma region 使用するアダプタを決定する
	//使用するアダプタ用の変数。最初にnullptrを入れておく
	
	//いい順にアダプタを積む
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) != DXGI_ERROR_NOT_FOUND; ++i) {
		//アダプターの情報取得
		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr));
		//ソフトウェアアダプタでなければ採用！
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			//ログに出力
			Log(ConvertString(std::format(L"Use Adapter:{}\n", adapterDesc.Description)));
			break;
		}
		useAdapter = nullptr;
	}
	//見つからなければ起動できない
	assert(useAdapter != nullptr);
#pragma endregion
#pragma region D3D12Deviceの生成

	//機能レベルとログと出力用の文字列
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0 };
	const char* featurelevelString[] = { "12.2","12.1","12.0" };
	//高い順に生成できるか試していく
	for (size_t i = 0; i < _countof(featureLevels); ++i) {
		//採用したアダプターでデバイスを生成
		hr = D3D12CreateDevice(useAdapter.Get(), featureLevels[i], IID_PPV_ARGS(&device));
		if (SUCCEEDED(hr)) {
			Log(std::format("FeatureLevel : {}\n", featurelevelString[i]));
			break;
		}
	}
	//デバイスの生成がうまく行かなかったので起動できない
	assert(device != nullptr);
	Log("Complete create D3D12Device!!\n");
#pragma region エラー・警告で停止
#ifdef _DEBUG
	
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		//やばいエラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		//エラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		if (isAssertForgetReleasing_) {
			//警告時に止まる
			infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
		}
#pragma region エラー警告の抑制
		D3D12_MESSAGE_ID denyIds[] = {
			//Windows11
			//https://stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows-11
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		//抑制するレベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		//指定したメッセージの表示を抑制する
		infoQueue->PushStorageFilter(&filter);
#pragma endregion
		
	}
#endif
#pragma endregion
#pragma endregion
}

void DirectXFunc::CommandInitialize()
{
#pragma region コマンドキューの生成
	//コマンドキューを生成	
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	HRESULT hr = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	//コマンドキューの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(hr));
#pragma endregion
#pragma region CommandListを生成
	//コマンドアロケータを生成する	
	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	assert(SUCCEEDED(hr));
	//コマンドリストを生成
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList));
	assert(SUCCEEDED(hr));
#pragma endregion
}

void DirectXFunc::SwapChainInitialize()
{
#pragma region SwapChainを生成する
	//スワップチェーンを生成する	
	swapChainDesc.Width = WindowApp::kClientWidth;
	swapChainDesc.Height = WindowApp::kClientHeight;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	HRESULT hr = dxgiFactory->CreateSwapChainForHwnd(commandQueue.Get(), winApp_->GetHwnd(), &swapChainDesc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain.GetAddressOf()));
	assert(SUCCEEDED(hr));
#pragma endregion
#pragma region SwapChain	
	hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&swapChainResources[0]));
	assert(SUCCEEDED(hr));
	hr = swapChain->GetBuffer(1, IID_PPV_ARGS(&swapChainResources[1]));
	assert(SUCCEEDED(hr));
#pragma endregion
}

void DirectXFunc::RTVInitialize()
{
	//RTVようのヒープでディスクリプタの数は２。RTVはSHADER内で触るものではないのでShaderVisibleはfalse
	rtvDescriptorHeap = CreateDescriptorHeap(device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);
	const uint32_t descriptorSizeRTV = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
#pragma region RTV
	//RTV
	
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	//ディスクリプタの先頭を取得する
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle = GetCPUDescriptorHandle(rtvDescriptorHeap, descriptorSizeRTV, 0);
	//
	
	//
	rtvHandles[0] = rtvStartHandle;
	device->CreateRenderTargetView(swapChainResources[0].Get(), &rtvDesc, rtvHandles[0]);
	//
	rtvHandles[1].ptr = rtvHandles[0].ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	device->CreateRenderTargetView(swapChainResources[1].Get(), &rtvDesc, rtvHandles[1]);
#pragma endregion
}



void DirectXFunc::DSVInitialize()
{
	
	//DSV用のヒープでディスクリプタの数は１。DSVはShader内で触るものではないので、ShaderVisibleはfalse
	dsvDescriptorHeap = CreateDescriptorHeap(device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);
	descriptorSizeDSV = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
#pragma region DSV
	//DepthStencilTextureをウィンドウサイズで作成
	depthStencilResource = CreateDepthStencilTextureResource(device.Get(), WindowApp::kClientWidth, WindowApp::kClientHeight);
	//DSVの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	//Format基本的にはResourceに合わせる
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;	//2dTexture

	//DSVHEapの先頭にDSVを作る
	device->CreateDepthStencilView(depthStencilResource, &dsvDesc, dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
#pragma endregion
}



void DirectXFunc::FenceInitialize()
{
#pragma region FenceとEventの生成と処理
	HRESULT hr = device->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(hr));

	//FenceのSignalを持つためのイベントを作成する
	fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);
#pragma endregion
}


#pragma endregion

void DirectXFunc::PreDraw()
{
	//これから書き込むバックバッファのインデックスを取得
	UINT backBufferIndex = swapChain->GetCurrentBackBufferIndex();
#pragma region TransitionBarrierを張る
	//Transitionbarrierの設定
	//今回のバリアはTransition
	barrier_.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	barrier_.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリソース、現在のバックバッファに対して行う
	barrier_.Transition.pResource = swapChainResources[backBufferIndex].Get();
	//遷移前（現在）のResourceState
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	//遷移後のResourceState
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//TransitionBarrierを張る
	commandList->ResourceBarrier(1, &barrier_);
#pragma endregion
#pragma region RTVとDSVの設定
	//描画先のRTVとDSVを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = GetCPUDescriptorHandle(dsvDescriptorHeap, descriptorSizeDSV, 0);
	commandList->OMSetRenderTargets(1, &rtvHandles[backBufferIndex], false, &dsvHandle);
#pragma endregion
	//指定した色で画面全体をクリアする
	float clearColor[] = { 0.1f,0.25f,0.5f,1.0f };//青っぽい色
	//指定した深度で画面全体をクリアする
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	commandList->ClearRenderTargetView(rtvHandles[backBufferIndex], clearColor, 0, nullptr);

#pragma region ViewportとScissor(シザー)
	//ビューポート
	D3D12_VIEWPORT viewport{};
	//クライアント領域のサイズと一緒にして画面全体に表示
	viewport.Width = (FLOAT)WindowApp::kClientWidth;
	viewport.Height = (FLOAT)WindowApp::kClientHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//シザー短形
	D3D12_RECT scissorRect{};
	//基本的にビューポートと同じ短形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = WindowApp::kClientWidth;
	scissorRect.top = 0;
	scissorRect.bottom = WindowApp::kClientHeight;
#pragma endregion
	commandList->RSSetViewports(1, &viewport);
	commandList->RSSetScissorRects(1, &scissorRect);

}

void DirectXFunc::PostDraw()
{
#pragma region 画面表示できるようにする
	//画面に描く処理はすべて終わり、画面に移すので状態を遷移
	//今回はRenderTargetからPresentにする
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	//TransitionBarrierを張る
	commandList->ResourceBarrier(1, &barrier_);
	//画面表示できるようにするおわり
#pragma endregion	

	KickCommand();
}

void DirectXFunc::KickCommand()
{
#pragma region コマンドをキックする
	//コマンドリストの内容を確定させる。すべてのコマンドを積んでからCloseすること
	HRESULT hr = commandList->Close();
	assert(SUCCEEDED(hr));
	//GPUにコマンドリストの実行を行わせる
	ID3D12CommandList* commandLists[] = { commandList.Get()};
	commandQueue->ExecuteCommandLists(1, commandLists);
	//GPUとOSに画面の交換を行うよう通知する
	swapChain->Present(1, 0);
	
#pragma region GPUにSignalをおくる
	//Fenceの値を更新
	fenceValue++;
	//GPUがここまでたどり着いたときに、Fenceの値を指定した値に代入するようにSignalを送る
	commandQueue->Signal(fence.Get(), fenceValue);
	//Fenceの値が指定したSignal値にたどり着いているか確認する
	//GetCompleteValueの初期値はFence作成時に渡した初期値
	if (fence->GetCompletedValue() < fenceValue) {
		//指定したSignalにたどり着いていないので、だどりつくまでイベントを指定する
		fence->SetEventOnCompletion(fenceValue, fenceEvent);
		//イベント待つ
		WaitForSingleObject(fenceEvent, INFINITE);
	}
	//GPUにシグナルを送るおわり
	UpdateFixFPS();
#pragma endregion 
	
	//次フレーム用のコマンドリストを取得
	hr = commandAllocator->Reset();
	assert(SUCCEEDED(hr));
	hr = commandList->Reset(commandAllocator.Get(), nullptr);
	assert(SUCCEEDED(hr));
	//コマンドをキックする終わり
#pragma endregion
}

void DirectXFunc::Finalize()
{
	CloseHandle(fenceEvent);

	depthStencilResource->Release();
	dsvDescriptorHeap->Release();
	
	rtvDescriptorHeap->Release();
}

