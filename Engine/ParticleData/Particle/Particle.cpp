#include"Particle/Particle.h"
#include"RandomNum/RandomNum.h"


Particle MakeNewParticle(const Vector3& spawnPos, const Vector3& emiterSize, const MinMaxDataV3& velo, const MinMaxDataV4& color, const Vector2& lifeTimeminXmaxY) {
	Particle ans;

	ans.world_.translate_ = spawnPos + RandomNumber::Get(-emiterSize / 2, emiterSize / 2);


	ans.velocity = RandomNumber::Get(velo.minData, velo.maxData);


	ans.color = RandomNumber::Get(color.minData, color.maxData);

	ans.lifeTime = RandomNumber::Get(lifeTimeminXmaxY.x, lifeTimeminXmaxY.y);
	ans.currentTime = 0;

	return ans;
}