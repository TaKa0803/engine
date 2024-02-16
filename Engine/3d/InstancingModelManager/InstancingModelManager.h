#pragma once

#include<string>
#include<vector>
#include<map>
#include<iostream>
#include"InstancingGameObject/InstancingModel.h"

class InstancingModelManager {
public:
	static InstancingModelManager* GetInstance();
private:
	InstancingModelManager() = default;
	~InstancingModelManager() = default;
	InstancingModelManager(const InstancingModelManager& o) = delete;
	const InstancingModelManager& operator=(const InstancingModelManager& o) = delete;

public:

	/// <summary>
	/// すべてのモデルのロード
	/// </summary>
	void LoadAllModel();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// 更新前処理
	/// </summary>
	void PreUpdate();

	/// <summary>
	/// すべてのモデルの描画
	/// </summary>
	void DrawAllModel(const Matrix4x4& viewProjection);


	/// <summary>
	/// タグの検索
	/// </summary>
	/// <param name="tag"></param>
	bool SerchTag(const std::string& tag);

	void Debug(const std::string& tag,  const char* name);
public:
	/// <summary>
	/// タグのモデルのα値取得
	/// </summary>
	/// <param name="tag"></param>
	/// <returns></returns>
	float GetAlpha(const std::string& tag);

public:

	/// <summary>
	/// タグのモデルにワールド追加
	/// </summary>
	/// <param name="tag">モデルタグ名</param>
	/// <param name="world">ワールドトランスフォーム</param>
	void SetData(const std::string& tag, const WorldTransform& world,const Vector4&color={1,1,1,1});

	/// <summary>
	/// タグのモデルに画像を設定
	/// </summary>
	/// <param name="tag">モデルタグ名</param>
	/// <param name="texture">画像の番号</param>
	void SetTexture(const std::string& tag, int texture);

	/// <summary>
	/// ブレンドモードの設定
	/// </summary>
	/// <param name="tag"></param>
	/// <param name="blend">kFillMode</param>
	void SetBlendMode(const std::string& tag, BlendMode blend);

	/// <summary>
	/// 埋めるモード設定
	/// </summary>
	/// <param name="tag">モデルタグ名</param>
	/// <param name="fillMode">kBlend</param>
	void SetFillMode(const std::string& tag, FillMode fillMode);

	/// <summary>
	/// α値設定
	/// </summary>
	/// <param name="tag"></param>
	/// <param name="alpha"></param>
	void SetAlpha(const std::string& tag, float alpha);

	/// <summary>
	/// 画像を有効にするか否か
	/// </summary>
	/// <param name="tag"></param>
	/// <param name="isEnable"></param>
	void SetEnableTexture(const std::string& tag, bool isEnable);

	/// <summary>
	/// シェーダーのONOFF
	/// </summary>
	/// <param name="tag">モデルタグ</param>
	/// <param name="isEnable">正否</param>
	void SetEnableShader(const std::string& tag, bool isEnable);
private:

	//jsonのパス
	const std::string modelPathFile_ = "resources/jsonfile/modelPathFile.json";

	//グループ名
	const std::string groupName_ = "InstancingmodelPathFile";

	//データ
	//std::vector<std::unique_ptr<InstancingModel>>modeldatas_;

	//
	std::map<std::string, std::unique_ptr<InstancingModel>>modelDatas_;

	//データの最大読み込み量
	const int maxModelNum = 256;
	//モデルの読み込んだ数
	int modelNum = 0;

};