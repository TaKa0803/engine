#pragma once
#include"InstancingGameObject/InstancingGameObject.h"

class Audience : public InstancingGameObject {


public:

	Audience();
	~Audience();

	void Initialize();

	void Update();

	void Draw();

	void Debug(const char* name);
private:
#pragma region MyRegion


	std::unique_ptr<InstancingGameObject>h_;
	std::unique_ptr<InstancingGameObject>B_;
	std::unique_ptr<InstancingGameObject>LA_;
	std::unique_ptr<InstancingGameObject>RA_;


	enum PARTS {
		HEAD,
		BODY,
		LARM,
		RARM,
		_count,
	};

	enum ANIMETYPE {
		NORMAL,
		GOOD,
		BAD,
		_countOfAnimeType
	};

	struct StEd {
		Vector3 st{};
		Vector3 ed{};
	};

	struct Parts {
		StEd pos;
		StEd rotate;
	};

	struct AnimeData {
		//パーツデータ
		Parts parts[_count];

		Vector3 pos[_count];
		Vector3 rotate[_count];


		//最大フレーム数
		int maxCount = 60;

		//フレームカウント
		int count = 0;

		//ループするか
		bool isLoop = true;
	};

	AnimeData animeData[_countOfAnimeType];

	AnimeData nowAnime;


	void Animation();
#pragma endregion

public:
	void SetAnimeType(ANIMETYPE type) { nowAnime = animeData[type]; }
};
