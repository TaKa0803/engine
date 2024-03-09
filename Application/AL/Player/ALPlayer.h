#pragma once
#include"SingleGameObjects/GameObject.h"
#include"Input/Input.h"
#include"Camera/Camera.h"
#include"SphereCollider/SphereCollider.h"
#include"Effect/Impact/Impact.h"
#include"AL/items.h"

#include<vector>



//攻撃のデータ構造体
struct ATKData {
	float extraTime = 0;	//予備動作
	float AttackTime = 0;	//攻撃動作
	float RigorTime = 0;	//硬直時間

	int isYATK = false;	//Yボタン攻撃か

	float spd = 0;

	ModelAnimeParts parts;

	std::vector<ATKData>ATKDerivation;	//攻撃の派生
};

class ALPlayer :public GameObject {

public:
	ALPlayer();
	~ALPlayer();

	void Initialize();

	void Update();

	void Draw();

	void DebugWindow(const char* name);

	void SetCamera(Camera* camera) { camera_ = camera; }


	SphereCollider* GetCollider() { return collider_.get(); }

	int GetConboCount() { return ATKConboCount; }

	bool IsPlayerATK() {
		if (state_ == State::ATK) { return true; }
		return false;
	}
private://メンバ関数


	//移動
	void Move();

	void ModelRoop(const Vector3& velo);

#pragma region 状態管理とメンバ関数ポインタテーブル

	enum class State {
		Move,		//移動
		ATK,		//攻撃
		HITACTION,	//被攻撃時
		SPECIALATK,	//特別攻撃
		kNumStates	//状態の数
	};

	//プレイヤーの状態
	State state_ = State::Move;
	//状態リクエスト
	std::optional<State>stateRequest_ = std::nullopt;

	//状態ごとの初期化テーブル
	static void (ALPlayer::* StateInitialize[])();

	//状態ごとの更新テーブル
	static void (ALPlayer::* StateUpdate[])();


	void InitializeMove();

	void InitializeATK();

	void InitializeHitAction();

	void InitializeSpecialATK();

	void UpdateMove();

	void UpdateATK();

	void UpdateHitAction();

	void UpdateSpecialATK();
#pragma endregion

	void LoadATKDatas();

private:
	Input* input_ = nullptr;

	const Camera* camera_ = nullptr;

	std::unique_ptr<SphereCollider> collider_;

#pragma region モデルに関する

	//タグ軍
	std::string tags[modelNum_] = {
		"Head",
		"LArm",
		"RArm",
		"LFoot",
		"RFoot"
	};

	enum Parts {
		HEAD,
		LARM,
		RARM,
		LFOOT,
		RFOOT
	};

	WorldTransform mWorlds[modelNum_] = {};

	std::unique_ptr<Model>models[modelNum_];

	int textureData = 0;
#pragma endregion

#pragma region 影
	std::unique_ptr<InstancingGameObject>shadow;
#pragma endregion


	//移動速度
	float spd_ = 0.5f;

	//ループアニメーション管理
	bool isRoopStart = false;

	enum RoopState {
		SetUp,
		Roop
	};



	RoopState roopState = SetUp;



	ModelAnimeParts seuUpAnime;

	//歩くアニメーション
	ModelAnimeParts walkData_;

	//止まったモーション
	ModelAnimeParts stopData_;

	ModelAnimeParts nowRoop_;

	int roopCount_ = 0;

	enum MoveState {
		StopS,
		MoveS,
		NoneS
	};

	MoveState moveState_ = NoneS;

#pragma region 攻撃に関する変数

	std::unique_ptr<EffectImpact>impactE_;

	//攻撃に関するデータ
	std::string atkDataPass_ = "resources/jsonfile/PlayerATKData.json";

	//グループ名
	std::string groupName_ = "BButtonATK";

	static const int itemNum = 2;

	//アイテムの名前
	std::string keyNames_[itemNum] = {
		"data",
		"ATKDerivation"
	};


	//ボタンを押したときからの攻撃一覧
	ATKData startATKData_;



	ModelAnimeParts ATKData1_;

	ModelAnimeParts ATKData2_;

	ModelAnimeParts ATKData3_;

	//最大コンボ数
	const int maxATKConBo = 3;

	int ATKConboCount = 1;

	enum class ATKState {
		Extra,//準備
		ATK,	//攻撃実行
		Rigor//硬直
	};
	//攻撃の状態enum
	ATKState atkState_;

	//攻撃時に使うデータまとめ
	struct ATKUpdateData {
		//次の攻撃をするか
		bool nextATK = false;
		bool isPushY = false;
		int count = 0;
	};

	//攻撃の更新データ
	ATKUpdateData updateATKData_;

	//実行する攻撃動作
	ATKData ATKData_;

	bool ATKAnimationSetup_ = false;

#pragma endregion

	int punchSound_;

	int kickSound_;

	int drilSound_;



};