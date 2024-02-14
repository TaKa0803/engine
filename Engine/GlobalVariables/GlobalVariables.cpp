#include "GlobalVariables.h"
#include"ImGuiManager/ImGuiManager.h"
#include<json.hpp>
#include<fstream>


GlobalVariables* GlobalVariables::GetInstance() { 
	static GlobalVariables Instance;
	return &Instance;
}

void GlobalVariables::CreateGroup(const std::string& groupName) { 
	datas_[groupName];
}

void GlobalVariables::SetValue(
	const std::string& groupName, const std::string& key, int32_t value) {
	//グループの参照を表示
	Group group = datas_[groupName];
	//新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	//設定した項目をstd::mapに追加
	group.items[key]=newItem;

	datas_[groupName] = group;
}
void GlobalVariables::SetValue(
    const std::string& groupName, const std::string& key, float value) {
	// グループの参照を表示
	Group group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;

	datas_[groupName] = group;
}
void GlobalVariables::SetValue(
    const std::string& groupName, const std::string& key, const Vector3& value) {
	// グループの参照を表示
	Group group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;

	datas_[groupName] = group;
}


void GlobalVariables::Update() {
	if (!ImGui::Begin("Gloval Variables", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}
	if (!ImGui::BeginMenuBar())return;
	
	
	for (std::map<std::string, Group>::iterator itGroup = datas_.begin(); itGroup != datas_.end();++itGroup) {
		// グループ名
		const std::string& groupName = itGroup->first;
	
		
		//グループの参照を取得
		Group& group = itGroup->second;
		if (!ImGui::BeginMenu(groupName.c_str()))
			continue;
		
		for (std::map<std::string, Item>::iterator itItem = group.items.begin();
		     itItem != group.items.end(); ++itItem) {
		//項目名を取得
			const std::string& itemName = itItem->first;
			//項目
			Item& item = itItem->second;

			if (std::holds_alternative<int32_t>(item.value)) {
				int32_t* ptr = std::get_if<int32_t>(&item.value);
				ImGui::SliderInt(itemName.c_str(), ptr, -100, 100);
			} else if (std::holds_alternative<float>(item.value)) {
				float* ptr = std::get_if<float>(&item.value);
				ImGui::SliderFloat(itemName.c_str(), ptr, -100, 100);
			} else if (std::holds_alternative<Vector3>(item.value)) {
				Vector3* ptr = std::get_if<Vector3>(&item.value);
				ImGui::SliderFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), -100, 100);
			}
		}

		// 改行
		ImGui::Text("\n");
		if (ImGui::Button("Save")) {
			SaveFile(groupName);
			std::string message = std::format("{}.json saved.", groupName);
			MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		}


		ImGui::EndMenu();
	}

	

	ImGui::EndMenuBar();
	ImGui::End();
}

void GlobalVariables::SaveFile(const std::string& groupName) {
	//グループを検索
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);
	//未登録チェック
	assert(itGroup != datas_.end());

	nlohmann::json root;
	root = nlohmann::json::object();

	//jsonオブジェスト登録
	root[groupName] = nlohmann::json::object();

	//各項目について
	for (std::map<std::string, Item>::iterator itItem = itGroup->second.items.begin();
	     itItem != itGroup->second.items.end(); ++itItem) {
	
		//項目名を取得
		const std::string& itemName = itItem->first;
		//項目の参照を取得
		Item& item = itItem->second;

		
		if (std::holds_alternative<int32_t>(item.value)) {//int型の値を保持していれば
			//int32_t型の値を登録
			root[groupName][itemName] = std::get<int32_t>(item.value);

		}else if (std::holds_alternative<float>(item.value)) {//float型の値を保持していれば
			// int32_t型の値を登録
			root[groupName][itemName] = std::get<float>(item.value);
		} else if (std::holds_alternative<Vector3>(item.value)) {//Vector3型の値を保持していれば
			Vector3 value = std::get<Vector3>(item.value);
			root[groupName][itemName] = nlohmann::json::array({value.x, value.y, value.z});
		}


	}

	//ディレクトリがなければ作成する
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists("Resources/GlobalVariables")) {
		std::filesystem::create_directory("Resources/GlobalVariables");
	}
	

	
	//書き込むJSONファイルのフルパスを合成する
	std::string filepath = kDirectoryPath + groupName + ".json";
	//書き込み用ファイルストリーム
	std::ofstream ofs;
	//ファイルを書き込み用に開く
	ofs.open(filepath);

	//ファイルオープン失敗?
	if (ofs.fail()) {
		std::string message = "Failed open file for write";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}
	//ファイルにjson文字列を書き込む
	ofs << std::setw(4) << root << std::endl;
	//ファイルを閉じる
	ofs.close();
	
}


void GlobalVariables::LoadFiles() {
	// ディレクトリがなければ返す
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists("Resources/GlobalVariables/")) {
		return;
	}

	std::filesystem::directory_iterator dir_it("Resources/GlobalVariables");
	for (const std::filesystem::directory_entry& entry : dir_it) {
		//ファイルパスを取得
		const std::filesystem::path& filepath = entry.path();

		//ファイル拡張子を取得
		std::string extention = filepath.extension().string();
		//.jsonファイル以外はスキップ
		if (extention.compare(".json") != 0) {
			continue;
		}
		LoadFile(filepath.stem().string());

	}
}
void GlobalVariables::LoadFile(const std::string& groupName) {
	//よみこむJSONファイルのフルパスを合成する
	std::string filepath = kDirectoryPath + groupName + ".json";
	//読み込み用ファイルストリーム
	std::ifstream ifs;
	//ファイルを読み込み用に聞く
	ifs.open(filepath);

	// ファイルオープン失敗?
	if (ifs.fail()) {
		std::string message = "Failed open file for write";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	nlohmann::json root;
	//json文字列からjsonのデータの構造に展開
	ifs >> root;
	//ファイルを閉じる
	ifs.close();

	//グループを検索
	nlohmann::json::iterator itGroup = root.find(groupName);

	//未登録チェック
	assert(itGroup != root.end());

	//各アイテムについて
	for (nlohmann::json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
	//アイテム名を取得
		const std::string& itemName = itItem.key();

		//int型を保持していた場合
		if (itItem->is_number_integer()) {
			int32_t value = itItem->get<int32_t>();
			SetValue(groupName, itemName, value);
		}else if (itItem->is_number_float()) {// float型を保持していた場合
			double value = itItem->get<double>();
			SetValue(groupName, itemName, static_cast<float>(value));
		} else if (itItem->is_array() && itItem->size() == 3) { // 要素数３の配列であれば
			Vector3 value = {itItem->at(0), itItem->at(1), itItem->at(2)};
			SetValue(groupName, itemName, value);
		}
		
	}
}


void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, int32_t value) {
	// よみこむJSONファイルのフルパスを合成する
	std::string filepath = kDirectoryPath + groupName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込み用に聞く
	ifs.open(filepath);

	// ファイルオープンなければ
	if (ifs.fail()) {
		SetValue(groupName, key, value);
		ifs.close();
		return;
	}

	nlohmann::json root;
	// json文字列からjsonのデータの構造に展開
	ifs >> root;
	ifs.close();

	// グループを検索
	nlohmann::json::iterator itGroup = root.find(groupName);

	// 未登録チェック
	if (itGroup == root.end()) {
		SetValue(groupName, key, value);
	}
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, float value) {
	// よみこむJSONファイルのフルパスを合成する
	std::string filepath = kDirectoryPath + groupName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込み用に聞く
	ifs.open(filepath);

	// ファイルオープンなければ
	if (ifs.fail()) {
		SetValue(groupName, key, value);
		ifs.close();
		return;
	}

	nlohmann::json root;
	// json文字列からjsonのデータの構造に展開
	ifs >> root;
	ifs.close();

	// グループを検索
	nlohmann::json::iterator itGroup = root.find(groupName);

	// 未登録チェック
	if (itGroup == root.end()) {
		SetValue(groupName, key, value);
	}
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key,const Vector3& value) {
	// よみこむJSONファイルのフルパスを合成する
	std::string filepath = kDirectoryPath + groupName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込み用に聞く
	ifs.open(filepath);

	// ファイルオープンなければ
	if (ifs.fail()) {
		SetValue(groupName, key, value);
		ifs.close();
		return;
	}

	nlohmann::json root;
	// json文字列からjsonのデータの構造に展開
	ifs >> root;
	ifs.close();

	// グループを検索
	nlohmann::json::iterator itGroup = root.find(groupName);

	// 未登録チェック
	if (itGroup == root.end()) {
		SetValue(groupName, key, value);
	}
}


int32_t GlobalVariables::GetIntvalue(const std::string& groupName, const std::string& key) const {
	// グループを検索
	std::map<std::string, Group>::const_iterator itGroup = datas_.find(groupName);
	// 未登録チェック
	assert(itGroup != datas_.end());

	const Group& group = itGroup->second;
	
	std::map<std::string, Item>::const_iterator itKey = group.items.find(key);
	//未登録チェック
	assert(itKey != group.items.end());

	return std::get<int32_t>(itKey->second.value);
}

float GlobalVariables::GetFloatvalue(const std::string& groupName, const std::string& key) const {
	// グループを検索
	std::map<std::string, Group>::const_iterator itGroup = datas_.find(groupName);
	// 未登録チェック
	assert(itGroup != datas_.end());

	const Group& group = itGroup->second;

	std::map<std::string, Item>::const_iterator itKey = group.items.find(key);
	// 未登録チェック
	assert(itKey != group.items.end());

	return std::get<float>(itKey->second.value);
}
Vector3 GlobalVariables::GetVector3value(const std::string& groupName, const std::string& key) const {
	// グループを検索
	std::map<std::string, Group>::const_iterator itGroup = datas_.find(groupName);
	// 未登録チェック
	assert(itGroup != datas_.end());

	const Group& group = itGroup->second;

	std::map<std::string, Item>::const_iterator itKey = group.items.find(key);
	// 未登録チェック
	assert(itKey != group.items.end());

	return std::get<Vector3>(itKey->second.value);
}