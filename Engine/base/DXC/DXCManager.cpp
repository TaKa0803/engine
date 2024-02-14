#include "DXCManager.h"

#include<cassert>

DXCManager* DXCManager::GetInstance()
{
	static DXCManager instance;
	return &instance;
}

void DXCManager::Initialize()
{
#pragma region DXCの初期化
	//dxcCompilerを初期化
	
	HRESULT hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr));

	//現時点でincludeはしないが、includeに対応するための設定
	hr = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr));
#pragma endregion
}
