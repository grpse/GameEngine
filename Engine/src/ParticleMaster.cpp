#include "ParticleMaster.h"

ParticleMaster::ParticleMaster() 
{
	
}

ParticleMaster::ParticleMaster(const ParticleMaster & other)
{
	mParticles = other.mParticles;
}

ParticleMaster::~ParticleMaster()
{

}

const ParticleMaster& ParticleMaster::operator=(const ParticleMaster& particleMaster)
{
	mParticles = particleMaster.mParticles;
	return *this;
}

void ParticleMaster::init()
{
	mParticles = new Particle[kMaxParticles];
}

void ParticleMaster::update()
{
	for (uint i = 0; i < kMaxParticles; i++) 
	{
		Particle& particle = mParticles[i];
		if (particle.isAlive()) {
			particle.update();
		}
		// else TODO: mark death buffer if it is dead to let particle be available on next requirement
	}
}

void ParticleMaster::instantiate(uint numberOfParticles, const Vector3& position, const Vector3& velocity, float rotation, float scale, float lifeLength, float gravityEffect)
{
	// find dead particles as the number of instances that we want
	for (uint i = 0; i < kMaxParticles && numberOfParticles > 0; i++, numberOfParticles--)
		if (!mParticles[i].isAlive()) {
			mParticles[i].setup(position, velocity, rotation, scale, lifeLength, gravityEffect);
		}

	// if still have particles to instantiate and can't enough alive, start using from the beginning
	// TODO: Make it mark dead particles found in the loop to avoid reinstantiating recently revived ones
}

void ParticleMaster::instantiateOne(const Vector3& position, const Vector3& velocity, float rotation, float scale, float lifeLength, float gravityEffect)
{
	bool noDeadParticlesFound = true;

	// find dead one dead particles and revive
	for (uint i = 0; i < kMaxParticles; i++)
		if (!mParticles[i].isAlive()) {
			mParticles[i].setup(position, velocity, rotation, scale, lifeLength, gravityEffect);
			noDeadParticlesFound = false;
			break;
		}

	// if no one found, take the first
	if (noDeadParticlesFound) {
		mParticles[0].setup(position, velocity, rotation, scale, lifeLength, gravityEffect);
	}
}

const Particle * ParticleMaster::getParticles() const
{
	return mParticles;
}

uint ParticleMaster::getParticlesCount() const
{
	return kMaxParticles;
}
