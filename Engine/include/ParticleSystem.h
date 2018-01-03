#pragma once
#include "LinearMath.h"
#include "ParticleMaster.h"
#include "Texture2D.h"
#include "Typedefs.h"

class ParticleSystem {

public:

	ParticleSystem(float pps, float speed, float gravityComplient, float lifeLength);

	static const float GRAVITY;

	void setProjectionMatrix(const Matrix4& projection);
	void generateParticles(Vector3 systemCenter);
	void emitParticle(Vector3 center);
	void loadTexture(const Texture2D& texture2d);
	void update();
	void render(const Camera& camera);

private:
	float mPps;
	float mSpeed;
	float mGravityComplient;
	float mLifeLength;
	Texture2D mTexture;
	
	ParticleMaster mParticleMaster;

	float random();
};