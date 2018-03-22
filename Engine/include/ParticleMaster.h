#pragma once
#include "LinearMath.h"
#include "Typedefs.h"

class Camera;
class Particle;
class Texture2D;

class ParticleMaster {

public:
	ParticleMaster();
	ParticleMaster(const ParticleMaster& other);
	~ParticleMaster();

	const ParticleMaster& operator=(const ParticleMaster& particleMaster);

	void init();
	void update();
	void instantiate(uint numberOfParticles, const Vector3& position, const Vector3& velocity, float rotation, float scale, float lifeLength, float gravityEffect);
	void instantiateOne(const Vector3& position, const Vector3& velocity, float rotation, float scale, float lifeLength, float gravityEffect);
	const Particle* getParticles() const;
	uint getParticlesCount() const;

private:
	Particle* mParticles;
	const uint kMaxParticles = 1000;
};