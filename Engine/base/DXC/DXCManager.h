#pragma once
#include<d3d12.h>
#include<dxcapi.h>
#include<wrl.h>

class DXCManager {
public://シングルトンパターン
	static DXCManager* GetInstance();


private:
	DXCManager() = default;
	~DXCManager() = default;
	DXCManager(const DXCManager& o) = delete;
	const DXCManager& operator=(const DXCManager& o) = delete;

public:
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Initialize();

	IDxcUtils* GetDxcUtils() { return dxcUtils; }
	IDxcCompiler3* GetDxcCompiler() { return dxcCompiler; }
	IDxcIncludeHandler* GetIncludeHandler() { return includeHandler; }
private:
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	IDxcIncludeHandler* includeHandler = nullptr;
};