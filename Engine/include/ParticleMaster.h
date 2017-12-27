#pragma once
#include "LinearMath.h"
#include "Camera.h"
#include "Typedefs.h"
#include "Particle.h"
#include "ParticleRenderer.h"
#include "Texture2D.h"

class ParticleMaster {

public:
	ParticleMaster();
	ParticleMaster(const ParticleMaster& other);
	~ParticleMaster();

	const ParticleMaster& operator=(const ParticleMaster& particleMaster);

	void init(const Matrix4 & projection);
	void update();
	void render(const Camera& camera);
	void instantiate(uint numberOfParticles, const Vector3& position, const Vector3& velocity, float rotation, float scale, float lifeLength, float gravityEffect);
	void instantiateOne(Texture2D& texture, const Vector3& position, const Vector3& velocity, float rotation, float scale, float lifeLength, float gravityEffect);

private:
	Particle* mParticles;
	ParticleRenderer mParticleRenderer;

	const uint kMaxParticles = 1000;
};