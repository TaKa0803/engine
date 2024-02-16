#pragma once
#include<string>
#include<map>
#include<wrl.h>
#include<xaudio2.h>
#include"AudioManager/SoundData.h"





//音声データ
struct SoundData {
	//波形フォーマット
	WAVEFORMATEX wfex;
	//バッファの先頭アドレス
	BYTE* pBuffer;
	//バッファのサイズ
	unsigned int bufferSize;
};


class AudioManager {

public://シングルトンパターン
	static AudioManager* GetInstance();

private://シングルトンパターン

	AudioManager() = default;
	~AudioManager() = default;
	AudioManager(const AudioManager& o) = delete;
	const AudioManager& operator=(const AudioManager& o) = delete;


public:

	/// <summary>
	/// タグから音声データ取得
	/// </summary>
	/// <param name="tag">タグ名</param>
	/// <returns></returns>
	static int LoadSoundNum(const std::string& tag);
	
	/// <summary>
	/// 音の再生
	/// </summary>
	/// <param name="dataNum">音声データへのnum</param>
	static void PlaySoundData(const int dataNum, float volume=1.0f, bool loop=false);

	/// <summary>
	/// 音のストップ
	/// </summary>
	/// <param name="num">音の番号</param>
	static void StopSound(const int num);

	//初期化
	void Initialize();

	//解放処理
	void Finalize();

	//全モデルの描画
	void LoadAllSoundData();

	void StopAllSounds();

private:
	//データ読み込み
	SoundData LoadSoundData(const char* name);

	//タグの音データのnum取得
	int LoadSoundNumFromTag(const std::string tag);

	//再生
	void Play(int num, float volume, bool loop);

	void Stop(int num);
private:

	//音楽の音声ファイル
	const std::string fileName_ = "resources/jsonfile/resourcePath.json";

	//ファイルのグループ名
	const std::string groupName_ = "audios";

	//サウンドフォルダまでのパス
	const std::string foldaPath ="resources/sounds/";

	Microsoft::WRL::ComPtr<IXAudio2> xAudio2=nullptr;

	IXAudio2MasteringVoice* masterVoice=nullptr;

	//音のデータの数
	int soundNum_ = 0;

	//音データ
	const int maxAudioData_ = 256;

	//要素の番号とタグ名の塊
	std::map<std::string,int>tagDatas_;
	//タグ名とデータの塊
	std::map<int, SoundData>soundDatas_;

	//再生中のデータ保存エリア
	std::map<int, IXAudio2SourceVoice*>playAudioDatas_;

};