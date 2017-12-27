#pragma once
#include "LinearMath.h"
#include "ParticleMaster.h"
#include "Texture2D.h"
#include "Typedefs.h"

class ParticleSystem {

public:

	ParticleSystem(float pps, float speed, float gravityComplient, float lifeLength);

	static const float GRAVITY;

	void generateParticles(Vector3 systemCenter);
	void emitParticle(Vector3 center);
	void setParticleMaster(ParticleMaster* particleMaster);
	void loadTexture(const char* filepath);

private:
	float mPps;
	float mSpeed;
	float mGravityComplient;
	float mLifeLength;
	Texture2D mTexture;
	
	ParticleMaster* mParticleMaster;

	float random();
};