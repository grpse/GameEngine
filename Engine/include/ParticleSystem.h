#pragma once
#include "LinearMath.h"
#include "ParticleMaster.h"


class ParticleSystem {

public:

	ParticleSystem(float pps, float speed, float gravityComplient, float lifeLength);

	static const float GRAVITY;
	void start();

	void generateParticles(Vector3 systemCenter);
	void emitParticle(Vector3 center);

	void setParticleMaster(const ParticleMaster& particleMaster);

private:
	float mPps;
	float mSpeed;
	float mGravityComplient;
	float mLifeLength;
	
	ParticleMaster mParticleMaster;

	float random();
};