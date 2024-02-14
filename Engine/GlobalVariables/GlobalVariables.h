#pragma once
#include<iostream>
#include<variant>
#include<map>
#include<string>
#include"Math/Vector3.h"

struct Item {
	std::variant<int32_t, float, Vector3> value;
};

struct Group {
	std::map<std::string, Item > items;
};



class GlobalVariables {
public:
	static GlobalVariables* GetInstance();

	void CreateGroup(const std::string& groupName);

	//値のセット(int)
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	// 値のセット(float)
	void SetValue(const std::string& groupName, const std::string& key, float value);
	// 値のセット(int)
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);
	
	/// <summary>
	/// 舞フレーム処理
	/// </summary>
	void Update();

	//データを保存
	void SaveFile(const std::string& groupName);

	//ファイル読み込み
	void LoadFiles();
	//ファイルよみこみ(１ファイル
	void LoadFile(const std::string& groupName);


	// 値の追加int)
	void AddItem(const std::string& groupName, const std::string& key, int32_t value);
	// 値のセット(float)
	void AddItem(const std::string& groupName, const std::string& key, float value);
	// 値のセット(int)
	void AddItem(const std::string& groupName, const std::string& key, const Vector3& value);


	//値の取得
	int32_t GetIntvalue(const std::string& groupName, const std::string& key) const;
	float GetFloatvalue(const std::string& groupName, const std::string& key) const;
	Vector3 GetVector3value(const std::string& groupName, const std::string& key) const;


private:

	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables& o) = delete;
	const GlobalVariables& operator=(const GlobalVariables& o) = delete;
	
	std::map<std::string, Group> datas_;

	//グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/GlobalVariables/";
	
};
