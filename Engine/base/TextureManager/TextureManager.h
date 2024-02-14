#pragma once
#include<string>
#include<vector>

#include<array>

#include"DirectXTex/d3dx12.h"

#include"DirectXTex/DirectXTex.h"


#include"DirectXFunc/DirectXFunc.h"

#include<wrl.h>
#include<map>

class TextureManager {
public://シングルトンパターン
	static TextureManager* GetInstance();

private:
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(const TextureManager& o) = delete;
	const TextureManager& operator=(const TextureManager& o) = delete;

public:

	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

	/// <summary>
	/// 仮利用の画像
	/// </summary>
	static int uvChecker_;


	/// <summary>
	/// 画像の読み込み処理
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	static int LoadTex(const std::string& filePath);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="DXF_"></param>
	void Initialize(DirectXFunc* DXF_);

	void Finalize();


private://メンバ関数
	/// <summary>
	/// データを作成
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <param name="mipImages"></param>
	/// <returns></returns>
	int CreateData(const std::string& filePath, const DirectX::ScratchImage& mipImages);

	/// <summary>
	/// 似たようなデータがないかチェック
	/// </summary>
	/// <param name="filepath">パス</param>
	/// <returns>あったか否か</returns>
	bool CheckSameData(const std::string& filepath);

	/// <summary>
	/// パスに合ったテクスチャハンドルへのイテレータ返却
	/// </summary>
	/// <param name="path">パス</param>
	/// <returns>イテレータ番号</returns>
	int GetDataFromPath(const std::string& path);
private://メンバ変数

	//
	DirectXFunc* DXF=nullptr;


	struct Texturedata {
		//パスに対応したイテレータ番号
		int texManagementNumber;
		//パス
		std::string filePath;
	};

	//データ群
	//std::vector<Texturedata*>datas_;

	std::map<std::string, int>tagNumDatas_;

	std::map<int, Texturedata*>texDatas_;

	std::string uvCheckerTex = "resources/SystemResources/uvChecker.png";

	//const size_t maxTexNum_ = 256;
};