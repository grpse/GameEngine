#pragma once
#include "LinearMath.h"
#include "ParticleMaster.h"
#include "ParticleTexture.h"
#include "Typedefs.h"
#include "SOIL.h"

class ParticleSystem {

public:

	ParticleSystem(float pps, float speed, float gravityComplient, float lifeLength);

	static const float GRAVITY;
	void start();

	void generateParticles(Vector3 systemCenter);
	void emitParticle(Vector3 center);

	void setParticleMaster(ParticleMaster* particleMaster);


	void loadTexture(const char* filepath) {
		int width, height, channels;
		byte* imageData = SOIL_load_image(filepath, &width, &height, &channels, SOIL_LOAD_RGBA);

		GLuint tex;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glGenerateMipmap(GL_TEXTURE_2D);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

		particleTexture.setup(tex, 1);
	}

private:
	float mPps;
	float mSpeed;
	float mGravityComplient;
	float mLifeLength;
	ParticleTexture particleTexture;
	
	ParticleMaster* mParticleMaster;

	float random();
};