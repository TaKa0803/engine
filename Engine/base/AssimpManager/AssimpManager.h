//#pragma once
//#include"struct.h"
//
//#include<vector>
//
//
//struct Vertex {
//	Vector3 Position;
//	Vector3 Normal;
//	Vector2 UV;
//	Vector3 Tangent;
//	Vector4 Color;
//};
//
//struct Mesh {
//	std::vector<Vertex>Vertices;
//	std::vector<uint32_t>Indices;
//	std::wstring DiffuseMap;
//};
//
//struct aiMesh;
//struct aiMaterial;
//
//struct ImportSettings {
//	const wchar_t* filename = nullptr;
//	std::vector<Mesh>& meshes;
//	bool inverseU = false;
//	bool inverseV = false;
//};
//
//class AssimpLoader {
//
//public:
//	bool Load(ImportSettings setting);//モデルをロード
//
//private:
//	//メッシュ読み込み
//	void LoadMesh(Mesh& dst, const aiMesh* src, bool inverseU, bool inverseV);
//	//画像読み込み
//	void LoadTexture(const wchar_t* filename, Mesh& dst, const aiMaterial* src);
//};