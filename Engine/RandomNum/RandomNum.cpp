#include "RandomNum.h"



RandomNumber* RandomNumber::GetInstance() {
	static RandomNumber instance;
	return&instance;
}

void RandomNumber::RandomNumberProcessInitialize() {
	std::random_device seedGenerator;
	std::mt19937_64 randomEngine(seedGenerator());

	randomEngine_ = randomEngine;
}

float RandomNumber::Get(const float minNum, const float maxNum) {
	std::uniform_real_distribution<float>distribution(minNum, maxNum);

	RandomNumber* rand = RandomNumber::GetInstance();

	return distribution(rand->GetrandEngine());
}

Vector2 RandomNumber::Get(const Vector2 minNum, const Vector2 maxNum) {
	std::uniform_real_distribution<float>distributionx(minNum.x, maxNum.x);
	std::uniform_real_distribution<float>distributiony(minNum.y, maxNum.y);

	RandomNumber* rand = RandomNumber::GetInstance();

	Vector2 ans = {
		distributionx(rand->GetrandEngine()),
		distributiony(rand->GetrandEngine())
	};

	return ans;
}

Vector3 RandomNumber::Get(const Vector3& minNum, const Vector3& maxNum) {

	RandomNumber* rand = RandomNumber::GetInstance();



	return {
		rand->GetRandomNum(minNum.x,maxNum.x),
		rand->GetRandomNum(minNum.y,maxNum.y),
		rand->GetRandomNum(minNum.z,maxNum.z),
	};
}

Vector4 RandomNumber::Get(const Vector4& minNum, const Vector4& maxNum) {
	RandomNumber* rand = RandomNumber::GetInstance();

	return {
		rand->GetRandomNum(minNum.x,maxNum.x),
		rand->GetRandomNum(minNum.y,maxNum.y),
		rand->GetRandomNum(minNum.z,maxNum.z),
		rand->GetRandomNum(minNum.w,maxNum.w)
	};
}

float RandomNumber::GetRandomNum(const float min, const float max) {
	float newmin;
	float newmax;

	if (min > max) {
		newmin = max;
		newmax = min;
	}
	else {
		newmin = min;
		newmax = max;
	}

	std::uniform_real_distribution<float>distribution(newmin, newmax);
	return distribution(randomEngine_);
}


