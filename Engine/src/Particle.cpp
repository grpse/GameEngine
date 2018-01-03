#include "Particle.h"
#include "ParticleSystem.h"
#include "Time.h"

Particle::Particle() 
{
	mIsAlive = false;
}

void Particle::setup(Vector3 position, Vector3 velocity, float rotation, float scale, float life, float gravityEffect)
{
	mOriginalPosition = mPosition  = position;
	mOriginalVelocity = mVelocity = velocity;
	mOriginalRotation = mRotation = rotation;
	mOriginalScale = mScale = scale;
	mOriginalLifeLength = mLifeLength = life;
	mOriginalGravityEffect = mGravityEffect = gravityEffect;
	mOriginalElapsedTime = mElapsedTime = 0;
	mIsAlive = true;
}

void Particle::update()
{
	if (isAlive()) {
		mVelocity.y += ParticleSystem::GRAVITY * mGravityEffect * Time::getDeltaTime();
		Vector3 change = mVelocity * (float)Time::getDeltaTime();
		mPosition += change;
		mElapsedTime += Time::getDeltaTime();
		mIsAlive = mElapsedTime < mLifeLength;
	}
}

bool Particle::isAlive()
{
	return mIsAlive;
}

void Particle::revive()
{
	mIsAlive = true;
	mPosition = mOriginalPosition;
	mVelocity = mOriginalVelocity;
	mRotation = mOriginalRotation;
	mScale = mOriginalScale;
	mLifeLength = mOriginalLifeLength;
	mGravityEffect = mOriginalGravityEffect;
	mElapsedTime = mOriginalElapsedTime;
}