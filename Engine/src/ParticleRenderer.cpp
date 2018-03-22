#include "Camera.h"
#include "Renderer.h"
#include "ParticleSystem.h"
#include <vector>
#include <iostream>
#include "ParticleRenderer.h"
#include "GLErrorHandling.h"
#include "ParticleBasicShader.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Particle.h"

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
	mShader.useProjectionMatrix();
	mShader.useWorldViewMatrix();

	mShader.buildShadersFromSource(ParticleShaderStr);

	mShader.start();
	mTextureUniformLocation = mShader.getUniformLocation("tex");
	mShader.stop();

	VertexBuffer vertexBuffer(quadVertices, sizeof(quadVertices));
	VertexBufferLayout layout;
	layout.pushFloat(3);
	mVertexArray.generateBuffer();
	mVertexArray.setVertexBuffer(vertexBuffer, layout);
}

ParticleRenderer::~ParticleRenderer()
{
	mVertexArray.deleteBuffer();
}

void ParticleRenderer::render(const ParticleSystem& particleSystem, const Camera & camera, const Renderer& renderer) const
{
	const Texture2D& texture2d = particleSystem.getTexture2D();
	const Matrix4& view = camera.getViewMatrix();
	uint particleCount = particleSystem.getParticlesCount();
	const Particle* particles = particleSystem.getParticles();

	prepare(renderer);
	
	mShader.setProjectionMatrix(camera.getProjectionMatrix());

	texture2d.start();
	mVertexArray.bind();

	while(particleCount--) {
		Particle particle = particles[particleCount];
		mShader.setUniform(mTextureUniformLocation, (int)0);

		Vector3 position = particles[particleCount].getPosition();
		float rotation = particles[particleCount].getRotation();
		float scale = particles[particleCount].getScale();
		updateModelViewMatrix(position, rotation, scale, view);

		renderer.renderQuad(0, 4);
	}
	
	mVertexArray.unbind();	
	texture2d.stop();
	finishRendering(renderer);
}

void ParticleRenderer::updateModelViewMatrix(const Vector3& position, float rotation, float scale, const Matrix4& view) const
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

void ParticleRenderer::prepare(const Renderer& renderer) const
{
	mShader.start();
	
	renderer.enableBlend();
	renderer.enableDepthTest();
	renderer.setBlendSrcAlpha_OneMinusSrcColor();
	renderer.unsetDepthMask();
}

void ParticleRenderer::finishRendering(const Renderer& renderer) const
{	
	renderer.setDepthMask();
	renderer.disableDepthTest();
	renderer.disableBlend();
	mShader.stop();
}
