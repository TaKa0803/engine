#include "ObjectPSO.h"
#include "ParticlePSO/ParticleRootSignature.h"
#include"DXC/DXCManager.h"

#include"Log/Log.h"
#include"functions/function.h"

#include<cassert>


#pragma region モデル用
ObjectPSO::ObjectPSO() {
}
ObjectPSO::~ObjectPSO() {
	
	for (int i = 0; i < int(FillMode::kCountOfFillMode); i++) {
		for (int h = 0; h < int(BlendMode::kCountOfBlendMode); h++) {
			graphicsPipelineState[i][h]->Release();
		}
	}
	
	/*
	for (auto& graphics : graphicsPipelineState) {
		for (auto& graphic : graphics) {
			graphic->Release();
		}
	}
	*/
}

void ObjectPSO::Initialize(ID3D12Device* device) {

#pragma region RootSignatureを生成する

			//RootSignatureの作成
			D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
			descriptionRootSignature.Flags =
				D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
#pragma region RootParameter 
			//RootParameter作成。PixelShaderのMAterialとVertexShaderのTransform
			D3D12_ROOT_PARAMETER rootParameters[5] = {};
			//
			rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;		//CBVを使う
			rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;		//PixelShaderで使う
			rootParameters[0].Descriptor.ShaderRegister = 0;						//レジスタ番号０とバインド

			//world
			rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;		//CBVを使う
			rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;	//PixelShaderで使う
			rootParameters[1].Descriptor.ShaderRegister = 0;						//レジスタ番号０とバインド

			//ライト
			rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[3].Descriptor.ShaderRegister = 1;

			//カメラ
			rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
			rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			rootParameters[4].Descriptor.ShaderRegister = 2;


#pragma region ディスクリプタレンジ
			D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
			descriptorRange[0].BaseShaderRegister = 0;								//0から始まる
			descriptorRange[0].NumDescriptors = 1;									//数
			descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;			//SRVを使う
			descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//offsetを自動計算	
#pragma endregion

#pragma region ディスクリプタテーブル
			//DescriptorTable
			rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;;		//DescriptorHeapを使う
			rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;					//PixelShaderで使う 
			rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;				//tableの中身の配列を指定
			rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);	//tableで利用する
#pragma endregion

			descriptionRootSignature.pParameters = rootParameters;					//ルートパラメータ配列へのポインタ
			descriptionRootSignature.NumParameters = _countof(rootParameters);		//配列の長さ

#pragma region Samplerの設定
			D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
			staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//バイニアリング
			staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
			staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
			staticSamplers[0].ShaderRegister = 0;
			staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			descriptionRootSignature.pStaticSamplers = staticSamplers;
			descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);
#pragma endregion

#pragma endregion
			//シリアライズしてバイナリにする
			ID3DBlob* signatureBlob = nullptr;
			ID3DBlob* errorBlob = nullptr;
			HRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
			if (FAILED(hr)) {
				Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
				assert(false);
			}
			//バイナリをもとに生成

			hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
			assert(SUCCEEDED(hr));


#pragma endregion
#pragma region InputLayoutの設定
			//InputLayout
			D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
			inputElementDescs[0].SemanticName = "POSITION";
			inputElementDescs[0].SemanticIndex = 0;
			inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

			inputElementDescs[1].SemanticName = "TEXCOORD";
			inputElementDescs[1].SemanticIndex = 0;
			inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
			inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

			inputElementDescs[2].SemanticName = "NORMAL";
			inputElementDescs[2].SemanticIndex = 0;
			inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

			D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
			inputLayoutDesc.pInputElementDescs = inputElementDescs;
			inputLayoutDesc.NumElements = _countof(inputElementDescs);
#pragma endregion
#pragma region ShaderをCompileする

			DXCManager* DXC = DXCManager::GetInstance();

			//Shaderをコンパイルする
			IDxcBlob* vertexShaderBlob = CompileShader(L"resources/shaders/Object3d.VS.hlsl", L"vs_6_0", DXC->GetDxcUtils(), DXC->GetDxcCompiler(), DXC->GetIncludeHandler());
			assert(vertexShaderBlob != nullptr);

			IDxcBlob* pixelShaderBlob = CompileShader(L"resources/shaders/Object3d.PS.hlsl", L"ps_6_0", DXC->GetDxcUtils(), DXC->GetDxcCompiler(), DXC->GetIncludeHandler());
			assert(pixelShaderBlob != nullptr);
#pragma endregion
#pragma region DepthStencilStateの設定を行う
			//DepthStencilStateの設定
			D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
			//Depthの機能を有効化する
			depthStencilDesc.DepthEnable = true;
			//書き込みします
			depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
			//比較関数はLessEqualつまりちかければ描画される
			depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
#pragma endregion

	for (int32_t fillNum = 0; fillNum < (int)FillMode::kCountOfFillMode; fillNum++) {
		for (int32_t blendNum = 0; blendNum < (int)BlendMode::kCountOfBlendMode; blendNum++) {
#pragma region PSO群
#pragma region BlendStateの設定を行う
			//BlendStateの設定
			D3D12_BLEND_DESC blendDesc{};

			switch (blendNum) {
			case 0:
				//ブレンドなし
				blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				
				break;
			case 1:
				//ノーマルブレンド
				blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				blendDesc.RenderTarget[0].BlendEnable = TRUE;
				blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
				blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
				blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
				blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
				blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
				blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

				break;
			case 2:
				//加算
				blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				blendDesc.RenderTarget[0].BlendEnable = TRUE;
				blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
				blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
				blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
				blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
				blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
				blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
				break;
			case 3:
				//減算(逆減算
				blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				blendDesc.RenderTarget[0].BlendEnable = TRUE;
				blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
				blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
				blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
				blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
				blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
				blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
				break;
			case 4:
				//乗算
				blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				blendDesc.RenderTarget[0].BlendEnable = TRUE;
				blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
				blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
				blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
				blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
				blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
				blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
				break;
			case 5:
				//スクリーン
				blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				blendDesc.RenderTarget[0].BlendEnable = TRUE;
				blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
				blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
				blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
				blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
				blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
				blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
				break;
			case 6:
				//ブレンドなし
				blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				break;
			default:
				break;
			}

#pragma endregion
#pragma region RasterizeStateの設定を行う
			//RasterizerState
			D3D12_RASTERIZER_DESC rasterizerDesc{};
			//裏面を表示しない
			rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;


			switch (fillNum) {
			case 0:
				//三角形の中を塗りつぶす
				rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
				break;
			case 1:
				//三角形の中を塗りつぶす
				rasterizerDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;
				break;
			default:
				rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
				break;
			}


#pragma endregion
#pragma region PSOを生成
			//psoDesc
			D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
			graphicsPipelineStateDesc.pRootSignature = rootSignature.Get();	//RootSignature
			graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;	//InputLayout
			graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),
				vertexShaderBlob->GetBufferSize() };
			graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),
				pixelShaderBlob->GetBufferSize() };
			graphicsPipelineStateDesc.BlendState = blendDesc;
			graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;
			//書き込むRTVの情報
			graphicsPipelineStateDesc.NumRenderTargets = 1;
			graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			//利用するトポロジのタイプ。三角形
			graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			//どのように画面に色を打ち込むかの設定
			graphicsPipelineStateDesc.SampleDesc.Count = 1;
			graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
			//DepthStencilの設定
			graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
			graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
			//実際に生成

			hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
				IID_PPV_ARGS(&graphicsPipelineState[fillNum][blendNum]));
			assert(SUCCEEDED(hr));
#pragma endregion
#pragma endregion
		}
	}
	Log("Complete GraphicsSystem Initialize\n");
}

void ObjectPSO::PreDraw(ID3D12GraphicsCommandList* commandList) {
	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	commandList->SetGraphicsRootSignature(rootSignature.Get());
	commandList->SetPipelineState(graphicsPipelineState[static_cast<int>(fillMode_)][static_cast<int>(blendMode_)]);
}

void ObjectPSO::SetBlendMode(BlendMode blend) {
	if (blendMode_ != blend) {
		blendMode_ = blend;
	}
}

#pragma endregion

#pragma region パーティクル用Graphics
ParticleGraphics::ParticleGraphics() {
}

ParticleGraphics::~ParticleGraphics() {
	for (int h = 0; h < static_cast<int>(BlendMode::kCountOfBlendMode); h++) {
		graphicsPipelineState[h]->Release();
	}
}

void ParticleGraphics::Initialize(ID3D12Device* device) {
	for (int32_t blendNum = 0; blendNum < (int)BlendMode::kCountOfBlendMode; blendNum++) {
#pragma region PSO群
#pragma region DXCの初期化
		//dxcCompilerを初期化
		IDxcUtils* dxcUtils = nullptr;
		IDxcCompiler3* dxcCompiler = nullptr;
		HRESULT hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
		assert(SUCCEEDED(hr));
		hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
		assert(SUCCEEDED(hr));

		//現時点でincludeはしないが、includeに対応するための設定
		IDxcIncludeHandler* includeHandler = nullptr;
		hr = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
		assert(SUCCEEDED(hr));
#pragma endregion
#pragma region RootSignatureを生成する
		//RootSignatureの作成
		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
		descriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
#pragma region RootParameter 
		//RootParameter作成。PixelShaderのMAterialとVertexShaderのTransform
		D3D12_ROOT_PARAMETER rootParameters[3] = {};
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;		//CBVを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;		//PixelShaderで使う
		rootParameters[0].Descriptor.ShaderRegister = 0;						//レジスタ番号０とバインド

		D3D12_DESCRIPTOR_RANGE descriptorRangeInstancing[1] = {};
		descriptorRangeInstancing[0].BaseShaderRegister = 0;
		descriptorRangeInstancing[0].NumDescriptors = 1;
		descriptorRangeInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descriptorRangeInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		//CBVを使う
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;	//PixelShaderで使う
		rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRangeInstancing;						//レジスタ番号０とバインド
		rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeInstancing);



#pragma region ディスクリプタレンジ
		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = 0;								//0から始まる
		descriptorRange[0].NumDescriptors = 1;									//数
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;			//SRVを使う
		descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//offsetを自動計算	
#pragma endregion

#pragma region ディスクリプタテーブル
		//DescriptorTable
		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;;		//DescriptorHeapを使う
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;					//PixelShaderで使う 
		rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;				//tableの中身の配列を指定
		rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);	//tableで利用する
#pragma endregion

		descriptionRootSignature.pParameters = rootParameters;					//ルートパラメータ配列へのポインタ
		descriptionRootSignature.NumParameters = _countof(rootParameters);		//配列の長さ

#pragma region Samplerの設定
		D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//バイニアリング
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
		staticSamplers[0].ShaderRegister = 0;
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		descriptionRootSignature.pStaticSamplers = staticSamplers;
		descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);
#pragma endregion

#pragma endregion
		//シリアライズしてバイナリにする
		ID3DBlob* signatureBlob = nullptr;
		ID3DBlob* errorBlob = nullptr;
		hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
		if (FAILED(hr)) {
			Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
			assert(false);
		}
		//バイナリをもとに生成

		hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
		assert(SUCCEEDED(hr));
#pragma endregion
#pragma region InputLayoutの設定
		//InputLayout
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
		inputElementDescs[0].SemanticName = "POSITION";
		inputElementDescs[0].SemanticIndex = 0;
		inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputElementDescs[1].SemanticName = "TEXCOORD";
		inputElementDescs[1].SemanticIndex = 0;
		inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputElementDescs[2].SemanticName = "NORMAL";
		inputElementDescs[2].SemanticIndex = 0;
		inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
		inputLayoutDesc.pInputElementDescs = inputElementDescs;
		inputLayoutDesc.NumElements = _countof(inputElementDescs);
#pragma endregion
#pragma region BlendStateの設定を行う
		//BlendStateの設定
		D3D12_BLEND_DESC blendDesc{};

		switch (blendNum) {
		case 0:
			//ブレンドなし
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

			break;
		case 1:
			//ノーマルブレンド
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

			break;
		case 2:
			//加算
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
			break;
		case 3:
			//減算(逆減算
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
			break;
		case 4:
			//乗算
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
			break;
		case 5:
			//スクリーン
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
			blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
			break;
		case 6:
			//ブレンドなし
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			break;
		default:
			break;
		}

#pragma endregion
#pragma region RasterizeStateの設定を行う
		//RasterizerState
		D3D12_RASTERIZER_DESC rasterizerDesc{};
		//裏面を表示しない
		rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;

		rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

#pragma endregion
#pragma region ShaderをCompileする
		//Shaderをコンパイルする
		IDxcBlob* vertexShaderBlob = CompileShader(L"resources/shaders/Particle.VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
		assert(vertexShaderBlob != nullptr);

		IDxcBlob* pixelShaderBlob = CompileShader(L"resources/shaders/Particle.PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
		assert(pixelShaderBlob != nullptr);
#pragma endregion
#pragma region DepthStencilStateの設定を行う
		//DepthStencilStateの設定
		D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
		//Depthの機能を有効化する
		depthStencilDesc.DepthEnable = true;
		//書き込みします
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		//比較関数はLessEqualつまりちかければ描画される
		depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
#pragma endregion
#pragma region PSOを生成
		//psoDesc
		D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

		graphicsPipelineStateDesc.pRootSignature = rootSignature.Get();	//RootSignature
		graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;	//InputLayout
		graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),
			vertexShaderBlob->GetBufferSize() };
		graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),
			pixelShaderBlob->GetBufferSize() };
		graphicsPipelineStateDesc.BlendState = blendDesc;
		graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;
		//書き込むRTVの情報
		graphicsPipelineStateDesc.NumRenderTargets = 1;
		graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		//利用するトポロジのタイプ。三角形
		graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		//どのように画面に色を打ち込むかの設定
		graphicsPipelineStateDesc.SampleDesc.Count = 1;
		graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		//DepthStencilの設定
		graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
		graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		//実際に生成

		hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
			IID_PPV_ARGS(&graphicsPipelineState[blendNum]));
		assert(SUCCEEDED(hr));
#pragma endregion
#pragma endregion
	}

	Log("Complete GraphicsSystem Initialize\n");
}

void ParticleGraphics::PreDraw(ID3D12GraphicsCommandList* commandList) {
	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	commandList->SetGraphicsRootSignature(rootSignature.Get());
	commandList->SetPipelineState(graphicsPipelineState[static_cast<int>(blendMode_)]);
}

void ParticleGraphics::SetBlendMode(BlendMode blend) {
	if (blendMode_ != blend) {
		blendMode_ = blend;
	}
}








#pragma endregion



