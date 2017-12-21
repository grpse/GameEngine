#pragma once
#include "LinearMath.h"
#include "ShaderProgram.h"
#include "Particle.h"
#include "Camera.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"


class ParticleRenderer {

public:
	ParticleRenderer();
	~ParticleRenderer();

	void init(const Matrix4 & projection);
	void render(const Particle particles[], uint particleCount, const Camera& camera);

private:
	ShaderProgram mShader;
	Matrix4 mProjection;
	VertexArray mVA;
	VertexBuffer mVB;
	uint mBuffer;


	void updateModelViewMatrix(const Vector3& position, float rotation, float scale, const Matrix4& view);
	void prepare();
	void finishRendering();
};