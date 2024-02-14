#pragma once
#include"struct.h"
#include<vector>
#include<string>
#include<iostream>

#include"WorldTransform/WorldTransform.h"
#include"DirectXFunc/DirectXFunc.h"
#include"struct.h"


class ModelManager {

public://シングルトンパターン
	static ModelManager* GetInstance();

private://シングルトンパターン

	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(const ModelManager& o) = delete;
	const ModelManager& operator=(const ModelManager& o) = delete;


public:	


	void LoadAllModels();

	

	/// <summary>
	/// モデルデータの取得
	/// </summary>
	/// <param name="filename">modelの名前またはresources以降のパス</param>
	/// <returns>モデルデータ</returns>
	ModelData GetModelData( const std::string& filename);
private:
	//モデルデータのパスが記入されたファイルへのパス
	const std::string& modelPathFile = "resources/modelPathFile.json";
	
	//モデルデータのパス群
	const std::string& groupName = "modelPathFile";

	//データ保存の構造体
	struct NameAndPath {
		std::string name;
		std::string path;
	};

	//保存データ構造
	std::vector<std::pair<NameAndPath, ModelData>> modelDatas;

	//最大量設定
	const size_t maxModelData = 256;

	//GetModelDataでモデルが見つからない場合エラーをはくか否か
	bool isError = true;
};

