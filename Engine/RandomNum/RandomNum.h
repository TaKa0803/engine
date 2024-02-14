#pragma once
#include<random>
#include"Math/Vector2.h"
#include"Math/Vector3.h"
#include"struct.h"

class RandomNumber {

public://シングルトンパターン
	static RandomNumber* GetInstance();

private://シングルトンパターン

	RandomNumber() = default;
	~RandomNumber() = default;
	RandomNumber(const RandomNumber& o) = delete;
	const RandomNumber& operator=(const RandomNumber& o) = delete;

public:

	//エンジンで呼び出しているのでほかで使う必要なし
	void RandomNumberProcessInitialize();

	/// <summary>
	/// ランダムな数字を取得
	/// </summary>
	/// <param name="minNum">最小値</param>
	/// <param name="maxNum">最大値</param>
	static float Get(const float minNum, const float maxNum);

	/// <summary>
	/// ランダムな数字を取得
	/// </summary>
	/// <param name="minNum">最小値</param>
	/// <param name="maxNum">最大値</param>
	/// <returns>ランダムな数字</returns>
	static Vector2 Get(const Vector2 minNum, const Vector2 maxNum);

	/// <summary>
	/// ランダムな数字を取得
	/// </summary>
	/// <param name="minNum">最小値</param>
	/// <param name="maxNum">最大値</param>
	/// <returns>ランダムな数字</returns>
	static Vector3 Get(const Vector3 &minNum, const Vector3& maxNum);

	/// <summary>
	/// ランダムな数字を取得
	/// </summary>
	/// <param name="minNum">最小値</param>
	/// <param name="maxNum">最大値</param>
	/// <returns>ランダムな数字</returns>
	static Vector4 Get(const Vector4& minNum, const Vector4& maxNum);


private:
	/// <summary>
	/// randomEngine取得
	/// </summary>
	/// <returns></returns>
	std::mt19937_64& GetrandEngine() { return randomEngine_; }

	float GetRandomNum(const float min, const float max);

	std::mt19937_64 randomEngine_;

	std::uniform_real_distribution<float>distribution(float x, float y);
	
};


