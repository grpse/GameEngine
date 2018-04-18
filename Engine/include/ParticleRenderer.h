#pragma once
#include "LinearMath.h"
#include "ShaderProgram.h"
#include "Particle.h"
#include "Camera.h"
#include "Texture2D.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture2D.h"

class Renderer;
class ParticleSystem;

class ParticleRenderer {

public:
	ParticleRenderer();
	~ParticleRenderer();
	void setup();
	void render(const ParticleSystem& particleSystem, const Camera & camera, const Renderer& renderer) const;

private:
	mutable ShaderProgram mShader;
	VertexArray mVertexArray;
	uint mTextureUniformLocation;

	void updateModelViewMatrix(const Vector3& position, float rotation, float scale, const Matrix4& view) const;
	void prepare(const Renderer& renderer) const;
	void finishRendering(const Renderer& renderer) const;
};