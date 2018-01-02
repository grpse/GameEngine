#include "ParticleSystem.h"
#include "Time.h"
#include "SOIL.h"
#include <cstdlib>

const float ParticleSystem::GRAVITY = -9.8f;

ParticleSystem::ParticleSystem(float pps, float speed, float gravityComplient, float lifeLength) {
	mPps = pps;
	mSpeed = speed;
	mGravityComplient = gravityComplient;
	mLifeLength = lifeLength;
}

void ParticleSystem::loadTexture(const char* filepath) {
	int width, height, channels;
	byte* imageData = SOIL_load_image(filepath, &width, &height, &channels, SOIL_LOAD_RGBA);

	Texture2D texture;
	texture.start();
	texture.setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	texture.setParameter(GL_TEXTURE_WRAP_T,  GL_CLAMP_TO_BORDER);
	texture.setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	texture.setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	texture.setFormat(GL_RGBA);
	texture.loadData(imageData, width, height);
	texture.stop();

	mParticleMaster->setTexture2D(texture);

	SOIL_free_image_data(imageData);
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

	mParticleMaster->instantiateOne(Vector3(center), velocity, 0, 1, mLifeLength, mGravityComplient);
}

void ParticleSystem::setParticleMaster(ParticleMaster* particleMaster) {
	mParticleMaster = particleMaster;
}

float ParticleSystem::random() 
{	
	float LO = -1;
	float HI = 1;
	return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
}