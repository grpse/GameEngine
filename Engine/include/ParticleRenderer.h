#pragma once
#include "LinearMath.h"
#include "ShaderProgram.h"
#include "Particle.h"
#include "Vbo.h"
#include "Camera.h"
#include "Attribute.h"
#include "Loader.h"

class ParticleRenderer {

public:
	ParticleRenderer();
	~ParticleRenderer();

	void init(const Matrix4 & projection);
	void render(const Particle particles[], uint particleCount, const Camera& camera);

private:
	ShaderProgram mShader;
	Matrix4 mProjection;
	//Vbo mQuad;
	//Attribute mQuadAttributes;
	uint mQuadVAO;
	uint mQuadBuffer;
	uint mQuadVertAttributeLocation;

	void updateModelViewMatrix(const Vector3& position, float rotation, float scale, const Matrix4& view);
	void prepare();
	void finishRendering();
};