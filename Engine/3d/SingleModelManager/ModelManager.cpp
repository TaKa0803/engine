#include "ModelManager.h"

#include<cassert>
#include<fstream>
#include<json.hpp>

#include<Windows.h>
#include<winuser.h>

#include"functions/function.h"
#include"TextureManager/TextureManager.h"
#include"SRVManager/SRVManager.h"


#pragma region ModelManager

ModelManager* ModelManager::GetInstance() {
	static ModelManager instance;
	return &instance;
}

void ModelManager::LoadAllModels() {
	//サイズを設定
	modelDatas.resize(maxModelData);

	//読み込むjsonファイル
	std::string filepath = modelPathFile;
	//読み込み用ファイルストリーム
	std::ifstream ifs;
	//ファイルを読み込み用に開く
	ifs.open(filepath);

	//ファイルオープン失敗処理
	if (ifs.fail()) {
		std::string message = "モデルパス記載のjsonファイルが見つかりません！";
		MessageBoxA(nullptr, message.c_str(), "ModelManager", 0);
		assert(message.c_str());
		return;
	}

	nlohmann::json root;
	//json文字列からjsonのデータ構造に展開
	ifs >> root;
	//移したのでファイルを閉じる
	ifs.close();

	//グループ検索
	nlohmann::json::iterator itGroup = root.find(groupName);

	//未登録チェック
	assert(itGroup != root.end());

	//各アイテムについて
	for (nlohmann::json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
		//アイテムの名前を取得
		const std::string& itemName = itItem.key();

		if (itItem->is_array() && itItem->size() == 2) {
			//モデル群の八田フォルダまでのパス
			std::string foldaPath = itItem->at(0);

			//パスを取得
			std::string modelPath = itItem->at(1);


			//名前とパスを合わせた構造体
			NameAndPath nameAndPath = { itemName,modelPath };

			//モデルデータを作成して設定
			ModelData newmodelData = LoadObjFile(foldaPath, modelPath);
			std::pair<NameAndPath, ModelData>newData(nameAndPath, newmodelData);
			modelDatas.emplace_back(newData);

		}
	}
}

ModelData ModelManager::GetModelData(const std::string& filename) {
	//フルパスを作成
	std::string name = filename;

	//データ型に該当するものを追加
	for (auto& modeldata : modelDatas) {
		//名前が同じorパスが同じ場合
		if (modeldata.first.name == name || modeldata.first.path == name) {
			return modeldata.second;
		}
	}
	//存在しない場合の処理
	if (isError) {
		//存在していないのでエラー
		assert(false);
	}
	else {
		//モデルデータを作成して設定
		ModelData newmodelData = LoadObjFile("resources", filename);
		std::pair<std::string, ModelData>newData(name, newmodelData);
		modelDatas.emplace_back(newData);
		return modelDatas.back().second;
	}


	return ModelData();
}
#pragma endregion


