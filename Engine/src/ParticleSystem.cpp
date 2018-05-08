#include <GL/glew.h>
#include "ParticleSystem.h"
#include "Time.h"
#include <cstdlib>

const float ParticleSystem::GRAVITY = -9.8f;

ParticleSystem::ParticleSystem(float pps, float speed, float gravityComplient, float lifeLength) {
	mPps = pps;
	mSpeed = speed;
	mGravityComplient = gravityComplient;
	mLifeLength = lifeLength;

	mParticleMaster.init();
}

void ParticleSystem::loadTexture(const Texture2D& texture2d)
{
	mTexture = texture2d;
	mTexture.bind();
	mTexture.setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	mTexture.setParameter(GL_TEXTURE_WRAP_T,  GL_CLAMP_TO_BORDER);
	mTexture.setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	mTexture.setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	mTexture.unbind();
}

void ParticleSystem::update()
{
	mParticleMaster.update();
}

const Particle * ParticleSystem::getParticles() const
{
	return mParticleMaster.getParticles();
}

uint ParticleSystem::getParticlesCount() const
{
	return mParticleMaster.getParticlesCount();
}

const Texture2D & ParticleSystem::getTexture2D() const
{
	return mTexture;
}

void ParticleSystem::generateParticles(Vector3 systemCenter) {
	float delta = (float)Time::getDeltaTime();
	float particlesToCreate = mPps * delta;
	int count = (int)floor(particlesToCreate);
	float partialParticle = ((uint)particlesToCreate) % 1;
	for (int i = 0; i<count; i++) {
		emitParticle(systemCenter);
	}

	if (random()  < partialParticle) {
		emitParticle(systemCenter);
	}
}

void ParticleSystem::emitParticle(Vector3 center) {
	float dirX = (float)random() * 2.0f - 1.0f;
	float dirZ = (float)random() * 2.0f - 1.0f;
	Vector3 velocity(dirX, 1, dirZ);
	velocity = Math::normalize(velocity);
	velocity *= mSpeed;

	mParticleMaster.instantiateOne(Vector3(center), velocity, 0, 1, mLifeLength, mGravityComplient);
}

float ParticleSystem::random() 
{	
	float LO = -1;
	float HI = 1;
	return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
}