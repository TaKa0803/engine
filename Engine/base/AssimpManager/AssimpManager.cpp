//#include"AssimpManager.h"
//
//#include<assimp/Importer.hpp>
//#include<assimp/scene.h>
//#include<assimp/postprocess.h>
//
//#include<d3d12.h>
//#include<filesystem>
//#include<cassert>
//
//namespace fs = std::filesystem;
//
//
//std::wstring GetDirectoryPath(const std::wstring& origin) {
//    fs::path p = origin.c_str();
//    return p.remove_filename().c_str();
//}
//
//std::string ToUTF8(const std::wstring& value) {
//    auto length = WideCharToMultiByte(CP_UTF8, 0U, value.data(), -1, nullptr, 0, nullptr, nullptr);
//    auto buffer = new char[length];
//
//    WideCharToMultiByte(CP_UTF8, 0U, value.data(), -1, buffer, length, nullptr, nullptr);
//
//    std::string result(buffer);
//    delete[] buffer;
//    buffer = nullptr;
//
//    return result;
//}
//
//// std::string(マルチバイト文字列)からstd::wstring(ワイド文字列)を得る
//std::wstring ToWideString(const std::string& str) {
//    auto num1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);
//
//    std::wstring wstr;
//    wstr.resize(num1);
//
//    auto num2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &wstr[0], num1);
//
//    assert(num1 == num2);
//    return wstr;
//}
//
//bool AssimpLoader::Load(ImportSettings setting) {
//
//    //ファイル名が指定されていない場合
//    if (setting.filename == nullptr) {
//        return false;
//    }
//
//    //各データ取得
//    auto& meshes = setting.meshes;
//    auto inverseU = setting.inverseU;
//    auto inverseV = setting.inverseV;
//
//    auto path = ToUTF8(setting.filename);
//
//    Assimp::Importer importer;
//
//    int flag = 0;
//    flag |= aiProcess_Triangulate;
//    flag |= aiProcess_PreTransformVertices;
//    flag |= aiProcess_CalcTangentSpace;
//    flag |= aiProcess_GenSmoothNormals;
//    flag |= aiProcess_GenUVCoords;
//    flag |= aiProcess_RemoveRedundantMaterials;
//    flag |= aiProcess_OptimizeMeshes;
//
//    //読み込み
//    auto scene = importer.ReadFile(path, flag);
//
//    if (scene == nullptr) {
//        // もし読み込みエラーがでたら表示する
//        assert(false);
//        return false;
//    }
//
//    // 読み込んだデータを自分で定義したMesh構造体に変換する
//    meshes.clear();
//    meshes.resize(scene->mNumMeshes);
//
//    for (size_t i = 0; i < meshes.size(); ++i) {
//        const auto pMesh = scene->mMeshes[i];
//        LoadMesh(meshes[i], pMesh, inverseU, inverseV);
//        const auto pMaterial = scene->mMaterials[i];
//        LoadTexture(setting.filename, meshes[i], pMaterial);
//    }
//
//    scene = nullptr;
//
//    return true;
//}
//
//void AssimpLoader::LoadMesh(Mesh& dst, const aiMesh* src, bool inverseU, bool inverseV) {
//
//    aiVector3D zero3D(0.0f, 0.0f, 0.0f);
//    aiColor4D zeroColor(0.0f, 0.0f, 0.0f, 0.0f);
//
//    dst.Vertices.resize(src->mNumVertices);
//
//
//    for (auto i = 0u; i < src->mNumVertices; ++i) {
//        auto position = &(src->mVertices[i]);
//        auto normal = &(src->mNormals[i]);
//        auto uv = (src->HasTextureCoords(0)) ? &(src->mTextureCoords[0][i]) : &zero3D;
//        auto tangent = (src->HasTangentsAndBitangents()) ? &(src->mTangents[i]) : &zero3D;
//        auto color = (src->HasVertexColors(0)) ? &(src->mColors[0][i]) : &zeroColor;
//
//        // 反転オプションがあったらUVを反転させる
//        if (inverseU) {
//            uv->x = 1 - uv->x;
//        }
//        if (inverseV) {
//            uv->y = 1 - uv->y;
//        }
//
//        Vertex vertex = {};
//        vertex.Position = Vector3(position->x, position->y, position->z);
//        vertex.Normal = Vector3(normal->x, normal->y, normal->z);
//        vertex.UV = Vector2(uv->x, uv->y);
//        vertex.Tangent = Vector3(tangent->x, tangent->y, tangent->z);
//        vertex.Color = Vector4(color->r, color->g, color->b, color->a);
//
//        dst.Vertices[i] = vertex;
//    }
//
//
//    dst.Indices.resize(src->mNumFaces * 3);
//
//    for (auto i = 0u; i < src->mNumFaces; ++i) {
//        const auto& face = src->mFaces[i];
//
//        dst.Indices[i * 3 + 0] = face.mIndices[0];
//        dst.Indices[i * 3 + 1] = face.mIndices[1];
//        dst.Indices[i * 3 + 2] = face.mIndices[2];
//    }
//
//
//}
//
//void AssimpLoader::LoadTexture(const wchar_t* filename, Mesh& dst, const aiMaterial* src) {
//    aiString path;
//    if (src->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS) {
//        // テクスチャパスは相対パスで入っているので、ファイルの場所とくっつける
//        auto dir = GetDirectoryPath(filename);
//        auto file = std::string(path.C_Str());
//        dst.DiffuseMap = dir + ToWideString(file);
//    }
//    else {
//        dst.DiffuseMap.clear();
//    }
//}
