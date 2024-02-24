#pragma once
#include<stdint.h>
#include<vector>
#include<string>

#include"Math/Vector2.h"
#include"Math/Vector3.h"
#include"Math/Vector4.h"
#include"Math/Matrix.h"


#pragma region 構造体

enum class BlendMode {
	//!<ブレンドなし
	kNone,
	//!<通常αブレンド
	kNormal,
	//!<加算
	kAdd,
	//!<減算
	kSubtract,
	//!<乗算
	kMultily,
	//!<スクリーン
	kScreen,
	//利用してはいけない
	kCountOfBlendMode
};

//塗りつぶし設定
enum class FillMode {
	//埋める
	kSolid,
	//ワイヤー
	kWireFrame,
	//使用不可
	kCountOfFillMode
};

struct WorldTransformation {
	Matrix4x4 WVP;
	Matrix4x4 World;
	Matrix4x4 WorldInverseTranspose;
	Vector4 color;
};

struct VertexData {
	Vector4 position = { 0,0,0,0 };
	Vector2 texcoord = { 0,0 };
	Vector3 normal = { 0,0,0 };
};

struct MaterialData {
	std::string textureFilePath;
};

struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
};









struct Material {
	//色
	Vector4 color;
	//UV座標
	Matrix4x4 uvTransform;

	int32_t enableLighting;
	//float padding[3];

	int32_t enableTexture;
	//float padding2[3];

	int32_t enableHalfLambert;
	//float padding3[3];

	float discardNum;
	//float padding4[3];

	int32_t enablePhongReflection = 1;

	//光沢度
	float shininess = 1.0f;

	int32_t enablePointLight=true;
};

struct DirectionalLight {
	Vector4 color;
	Vector3 direction;
	float intensity;
	//float padding2[3];
};

struct Camera4GPU {
	Vector3 worldPosition;
};

struct Segment {
	Vector3 origin;
	Vector3 diff;
};


//ポイントライト
struct PointLight
{
	Vector4 color;//ライトの色
	Vector3 position;//座標
	float intensity;//光度
};

#pragma endregion
