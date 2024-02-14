#include"AudioManager/AudioManager.h"


#pragma comment(lib, "xaudio2.lib")
#include<fstream>
#include<json.hpp>
#include<cassert>


struct ChunkHeader {
	char id[4];		//チャンク用のID
	int32_t size;	//チャンクサイズ
};
struct RiffHeader {
	ChunkHeader chunk;//RIFF
	char type[4];//WAVE
};
struct FormatChunk {
	ChunkHeader chunk;//ｆｍｔ
	WAVEFORMATEX fmt;//波形フォーマット
};

AudioManager* AudioManager::GetInstance()
{
	static AudioManager instance;
	return &instance;
}

void AudioManager::Finalize()
{
	//インスタンス開放
	xAudio2.Reset();

	//音声データ解放
	for (auto& data : soundDatas_) {
		delete[] data.second.pBuffer;

		data.second.pBuffer = 0;
		data.second.bufferSize = 0;
		data.second.wfex = {};
	}

	CoUninitialize();
}



int AudioManager::LoadSoundNum(const std::string& tag)
{
	return AudioManager::GetInstance()->LoadSoundNumFromTag(tag);
}

void AudioManager::StopSound(const int num)
{
	AudioManager::GetInstance()->Stop(num);
}

void AudioManager::Initialize()
{
	// オーディオの初期設定
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));
	hr = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(hr));
	hr = xAudio2->CreateMasteringVoice(&masterVoice);
}



void AudioManager::LoadAllSoundData()
{
	//読み込むjsonファイル
	std::string filepath = fileName_;
	//読み込み用ファイルストリーム
	std::ifstream ifs;
	//ファイルを読み込み用に開く
	ifs.open(filepath);

	//ファイルオープン失敗処理
	if (ifs.fail()) {
		std::string message = "soundData jsonfile is not find！";
		MessageBoxA(nullptr, message.c_str(), "SoundManager", 0);
		assert(message.c_str());
		return;
	}

	nlohmann::json root;
	//json文字列からjsonのデータ構造に展開
	ifs >> root;
	//移したのでファイルを閉じる
	ifs.close();

	//グループ検索
	nlohmann::json::iterator itGroup = root.find(groupName_);

	//未登録チェック
	assert(itGroup != root.end());

	//各アイテムについて
	for (nlohmann::json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {		
			//タグの名前を取得
			const std::string& itemName = itItem.key();
			
			//文字列かチェック
			if (itItem->is_array()) {
				//音のwavデータまでのパス
				std::string path = (std::string)itItem->at(0);

				std::string fullpath = foldaPath + path;

				//モデルデータを作成して設定
				SoundData soundData = LoadSoundData(fullpath.c_str());

				//タグに対応する要素番号取得
				tagDatas_[itemName] = soundNum_;
				soundDatas_[soundNum_] = soundData;

				//カウンター増加
				soundNum_++;
			}
	}
}

void AudioManager::StopAllSounds()
{
	for (auto& playS : playAudioDatas_) {
		playS.second->Stop();
		playS.second->FlushSourceBuffers();
		playS.second->DestroyVoice();

		playS.second=nullptr;

		
	}

	playAudioDatas_.clear();
}

 SoundData AudioManager::LoadSoundData(const char* name)
{

#pragma region ファイルオープン
	std::ifstream fs;
	fs.open(name, std::ios_base::binary);
	assert(fs.is_open());
#pragma endregion

#pragma region wavデータ読み込み
	RiffHeader riff;
	fs.read((char*)&riff, sizeof(riff));
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		//RIFFではない
		assert(0);
	}
	if (strncmp((char*)&riff.type, "WAVE", 4) != 0) {
		//waveじゃない
		assert(0);
	}

	//フォーマットチャンクの読み込み
	FormatChunk format = {};

	//チャンクヘッダーの確認
	while (true)
	{
		fs.read((char*)&format, sizeof(ChunkHeader));
		if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
			if (strncmp(format.chunk.id, "JUNK", 4) == 0) {
				fs.seekg(format.chunk.size, std::ios_base::cur);
			}
			//assert(0);
		}
		else {
			break;
		}
	}
	//チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	fs.read((char*)&format.fmt, format.chunk.size);

	//データチャンクの読み込み
	ChunkHeader data;
	fs.read((char*)&data, sizeof(data));
	//JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0) {
		fs.seekg(data.size, std::ios_base::cur);
		//再読み込み
		fs.read((char*)&data, sizeof(data));
	}


	//
	//if (strncmp(data.id, "data", 4) != 0) {
	//	//データなし
	//	assert(0);
	//}

	while (true) {
		if (strncmp(data.id, "data", 4) != 0) {
			fs.seekg(data.size, std::ios_base::cur);
			//再読み込み
			fs.read((char*)&data, sizeof(data));
		}
		else {
			break;
		}
	}

	//データチャンクのデータ部の読み込み
	char* pBuffer = new char[data.size];
	fs.read(pBuffer, data.size);

	fs.close();
#pragma endregion
#pragma region 読み込んだデータを返す
	SoundData soundData = {};
	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;
#pragma endregion

	return soundData;
}

 int AudioManager::LoadSoundNumFromTag(const std::string tag)
 {
	 return tagDatas_[tag];
 }

 void AudioManager::Play(int num,bool loop)
 {
	 //要素番号のタグからデータ取得
	 SoundData data = soundDatas_[num];

	 IXAudio2SourceVoice* pSourceVoice = nullptr;
	 HRESULT hr = xAudio2->CreateSourceVoice(&pSourceVoice, &data.wfex);
	 //作成失敗
	 assert(SUCCEEDED(hr));

	 pSourceVoice->SetVolume(0.5f);
	 

	 XAUDIO2_BUFFER buf{};
	 buf.pAudioData = data.pBuffer;
	 buf.AudioBytes = data.bufferSize;
	 buf.Flags = XAUDIO2_END_OF_STREAM;
	 if (loop) {
		 buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	 }
	 hr = pSourceVoice->SubmitSourceBuffer(&buf);
	 hr = pSourceVoice->Start();

	 assert(SUCCEEDED(hr));

	 playAudioDatas_[num] = pSourceVoice;
 }

 void AudioManager::Stop(int num)
 {
	 playAudioDatas_[num]->Stop();
	 playAudioDatas_[num]->FlushSourceBuffers();
	 playAudioDatas_[num]->DestroyVoice();

	 playAudioDatas_[num] = nullptr;

	 //キーとデータ削除
	 playAudioDatas_.erase(num);
 }



void AudioManager::PlaySoundData(const int dataNum, bool loop)
{

	AudioManager* AM = AudioManager::GetInstance();

	AM->Play(dataNum,loop);

}


