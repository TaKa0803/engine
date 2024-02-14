#pragma once

#include<wrl.h>

#define DIRECTINPUT_VERSION 0x0800 //バージョン設定
#include<dinput.h>
#include <XInput.h>
#include"Math/Vector2.h"
#include"Math/Vector3.h"
#include"WinApp/WinApp.h"

enum kPadButton {
	kButtonA,
	kButtonB,
	kButtonX,
	kButtonY,
	kLeftTrigger,
	kRightTrigger,

	kUp,
	kDown,
	kLeft,
	kRight
};

class Input {
public:

	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

	static Input* GetInstance();

private:
	Input() = default;
	~Input() = default;
	Input(const Input& o) = delete;
	const Input& operator=(const Input& o) = delete;

public:

	void Initialize(WindowApp*winApp);

	void Update();

	bool PushKey(BYTE keyNum);

	bool TriggerKey(BYTE keyNum);

	//WASD(XZ面
	Vector3 GetWASD();

	Vector3 GetAllArrowKey();


	//コントローラーが接続されているか
	bool IsControllerActive();

	//どれかボタンを押したか
	bool IsPushSomeButtons();

	//ジョイスティック右取得
	Vector2 GetjoyStickR();

	//ジョイスティック左取得
	Vector2 GetjoyStickL();

	//yを0にしたxzで値を変換
	Vector3 GetjoyStickLV3();

	bool IsPushButton(kPadButton kButton);

	bool IsTriggerButton(kPadButton kButton);

	void SetDeadLine(float deadLine) { deadLine_ = deadLine; }
private:

	//キーボードデバイス
	ComPtr<IDirectInputDevice8>keyboad;

	//キー入力
	BYTE key[256] = {};

	//1フレーム前キー入力
	BYTE preKey[256] = {};

	//

	//コントローラー入力
	XINPUT_STATE joyState;

	//1フレーム前コントローラー入力
	XINPUT_STATE preJoyState_;

	//デッドライン
	float deadLine_ = 0.2f;
};