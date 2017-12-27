#include <GL/glew.h>
#include <vector>
#include <iostream>
#include "ParticleRenderer.h"
#include "GLErrorHandling.h"
#include "ParticleBasicShader.h"

static Vector3 quadVertices[] = {
	Vector3(-0.5, -0.5, 0),
	Vector3( 0.5, -0.5, 0),
	Vector3( 0.5, 0.5, 0),
	Vector3(-0.5, 0.5, 0),
};

static Vector2 uvs[] = {
	Vector2(0, 0),
	Vector2(1, 0),
	Vector2(1, 1),
	Vector2(0, 1)
};

ParticleRenderer::ParticleRenderer()
{
	mShader.useVertexAttribute();
	mShader.useTextureCoord0Attribute();
	mShader.useProjectionMatrix();
	mShader.useWorldViewMatrix();

	mShader.buildShadersFromSource(ParticleShaderStr);

	GLCall(glGenBuffers(1, &mBuffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mBuffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW));
}

ParticleRenderer::~ParticleRenderer()
{

}

void ParticleRenderer::init(const Matrix4 & projection)
{
	mProjection = projection;

	mShader.start();
	mShader.setProjectionMatrix(mProjection);
	mShader.stop();
}

void ParticleRenderer::render(const Particle particles[], uint particleCount, const Camera & camera)
{
	Matrix4 view = camera.createViewMatrix();
	prepare();
	mShader.setProjectionMatrix(mProjection);

	uint texUniform = mShader.getUniformLocation("tex");
	while(particleCount--) {
		Particle particle = particles[particleCount];
		mShader.start();
		particle.getTexture().start();
		mShader.setUniform(texUniform, 0);

		Vector3 position = particles[particleCount].getPosition();
		float rotation = particles[particleCount].getRotation();
		float scale = particles[particleCount].getScale();
		updateModelViewMatrix(position, rotation, scale, view);

		//uint attributeLocation = mShader.getAttributeLocation("VertexPosition_ModelSpace");
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, mBuffer));
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void*)0));

		GLCall(glDrawArrays(GL_QUADS, 0, 4));

		particle.getTexture().stop();
	}

	finishRendering();
}

void ParticleRenderer::updateModelViewMatrix(const Vector3& position, float rotation, float scale, const Matrix4& view)
{
	Matrix4 world = Math::translate(position);
	
	// clear rotation on matrix multiplication
	world[0][0] = view[0][0];
	world[1][0] = view[0][1];
	world[2][0] = view[0][2];
	
	world[0][1] = view[1][0];
	world[1][1] = view[1][1];
	world[2][1] = view[1][2];

	world[0][2] = view[2][0];
	world[1][2] = view[2][1];
	world[2][2] = view[2][2];

	// get world view matrix
	Matrix4 worldView = view * world;

	// rotate (camera facing) only in z
	world = Math::rotate(world, Math::radians(rotation), Vector3(0, 0, 1));
	world = Math::scale(world, Vector3(scale, scale, scale));


	mShader.setWorldViewMatrix(worldView);
}

void ParticleRenderer::prepare()
{
	mShader.start();
	
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR));
	GLCall(glDepthMask(GL_FALSE));
	
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mBuffer));
}

void ParticleRenderer::finishRendering()
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	
	GLCall(glDepthMask(GL_TRUE));
	GLCall(glDisable(GL_BLEND));
	mShader.stop();
}
