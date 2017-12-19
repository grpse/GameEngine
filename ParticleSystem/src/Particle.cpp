#include "Particle.h"
#include "ParticleSystem.h"
#include "Time.h"

Particle::Particle(Vector3 position, Vector3 velocity, float rotation, float scale, float life, float gravityEffect)
	: mPosition(position), mVelocity(velocity), mRotation(rotation), mScale(scale), mLifeLength(life), mGravityEffect(gravityEffect)
{
}

void Particle::update()
{
	this->mVelocity.y += ParticleSystem::GRAVITY * mGravityEffect * Time::getDeltaTime();
		 
}
