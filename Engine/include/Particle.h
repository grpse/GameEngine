#pragma once
#include "LinearMath.h"
#include "ParticleTexture.h"

class Particle {

public:
	Particle();
	void setup(Vector3 position, Vector3 velocity, float rotation, float scale, float lifeLength, float gravityEffect);

	inline Vector3 getPosition() const {
		return mPosition;
	}

	inline float getRotation() const {
		return mRotation;
	}

	inline float getScale() const {
		return mScale;
	}

	void setTexture(ParticleTexture& texture);

	ParticleTexture getTexture() {
		return texture;
	}

	void update();
	bool isAlive();
	void revive();

private:
	bool mIsAlive;

	 ParticleTexture texture;

	Vector3 mPosition;
	Vector3 mVelocity;
	float	mRotation;
	float	mScale;
	float	mLifeLength;
	float	mGravityEffect;
	float	mElapsedTime;

	Vector3 mOriginalPosition;
	Vector3 mOriginalVelocity;
	float	mOriginalRotation;
	float	mOriginalScale;
	float	mOriginalLifeLength;
	float	mOriginalGravityEffect;
	float	mOriginalElapsedTime;

};