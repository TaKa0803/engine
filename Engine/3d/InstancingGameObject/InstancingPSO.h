#pragma once

#include<string>
#include<d3d12.h>
#include<dxcapi.h>
#include<wrl.h>
#include<vector>

#include"struct.h"



class InstancingPSO {

public:
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

	InstancingPSO();
	~InstancingPSO();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	void Initialize(ID3D12Device* device);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	void PreDraw(ID3D12GraphicsCommandList* commandList, const FillMode& fillMode, const BlendMode& blendMode);

private:


private:

	std::wstring vsPass = L"resources/shaders/InstancingObject.VS.hlsl";
	std::wstring psPass = L"resources/shaders/InstancingObject.PS.hlsl";

	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature = nullptr;

	//グラフィックパイプライン
	ID3D12PipelineState* graphicsPipelineState[int(FillMode::kCountOfFillMode)][int(BlendMode::kCountOfBlendMode)] = { nullptr };

	

	bool isInitialize_ = false;
};
