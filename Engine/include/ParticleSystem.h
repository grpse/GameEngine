#pragma once
#include "LinearMath.h"
#include "ParticleMaster.h"
#include "Typedefs.h"
#include "Texture2D.h"

class ParticleSystem {

public:

	ParticleSystem(float pps, float speed, float gravityComplient, float lifeLength);

	static const float GRAVITY;

	void generateParticles(Vector3 systemCenter);
	void emitParticle(Vector3 center);
	void loadTexture(const Texture2D& texture2d);
	void update();
	const Particle* getParticles() const;
	uint getParticlesCount() const;
	const Texture2D& getTexture2D() const;

private:
	float mPps;
	float mSpeed;
	float mGravityComplient;
	float mLifeLength;
	Texture2D mTexture;
	ParticleMaster mParticleMaster;

	float random();
};