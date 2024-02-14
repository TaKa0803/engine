#pragma once

#include<Windows.h>


class WindowApp {
public://シングルトンパターン
	static WindowApp* GetInstance();

private://シングルトンパターン

	WindowApp() = default;
	~WindowApp() = default;
	WindowApp(const WindowApp& o) = delete;
	const WindowApp& operator=(const WindowApp& o) = delete;


public://静的メンバ変数
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		
	//
	static int kClientWidth ;
	static int kClientHeight;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(LPCWSTR windowName, int clientwidth = 1280, int clientHeight=720);

	
	/// <summary>
	/// 消す前の処理
	/// </summary>
	void Finalize();
/// <summary>
/// プロセスメッセージ
/// </summary>
	bool ProcessMessage();

	HWND GetHwnd()const { return hwnd_; }

	WNDCLASS GetWc()const { return wc; }
private:
	WNDCLASS wc{};
	//ウィンドウハンドル
	HWND hwnd_ = nullptr;
};