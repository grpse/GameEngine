#pragma once
#include "LinearMath.h"

class Particle {

public:
	Particle(Vector3 position, Vector3 velocity, float rotation, float scale, float lifeLength, float gravityEffect);

	inline Vector3 getPosition() {
		return mPosition;
	}

	inline float getRotation() {
		return mRotation;
	}

	inline float getScale() {
		return mScale;
	}

private:
	Vector3 mPosition;
	Vector3 mVelocity;
	float mRotation;
	float mScale;
	float mLifeLength;
	float mGravityEffect;
	float mElapsedTime;

	void update();
};