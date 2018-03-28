#include <GL/glew.h>
#include "ShadowRenderer.h"

#include "Camera.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Transform.h"
#include "ShaderProgram.h"
#include "Light.h"
#include "FrameBuffer.h"
#include "Texture2D.h"
#include "ShadowShaders.h"

ShadowRenderer::ShadowRenderer()
{
	// Shadow map shader to generate depth texture
	mShadowMapShader.useVertexAttribute();
	mShadowMapShader.buildShadersFromSource(ShadowShader);

	mShadowMapShader.start();
	mDepthMVPLocation = mShadowMapShader.getUniformLocation("depthMVP");
	mShadowMapShader.stop();

	// Additive Shadow Shader to add Shadow to Scene
	mShadowMapAdditiveShader.useVertexAttribute();
	mShadowMapAdditiveShader.useWorldViewProjectionMatrix();

	mShadowMapAdditiveShader.buildShadersFromSource(ShadowMapShaderAdditive);

	mShadowMapAdditiveShader.start();
	mDepthMVPBiasLocation = mShadowMapAdditiveShader.getUniformLocation("DepthMVPBias");
	mShadowIntensityLocation = mShadowMapAdditiveShader.getUniformLocation("ShadowIntensity");
	mShadowMapLocation = mShadowMapAdditiveShader.getUniformLocation("ShadowMap");
	mShadowMapAdditiveShader.stop();

	DepthLayout.target = GL_TEXTURE_2D;
	DepthLayout.level = 0;
	DepthLayout.internalFormat = GL_DEPTH_COMPONENT16;
	DepthLayout.width = 1024;
	DepthLayout.height = 1024;
	DepthLayout.border = 0;
	DepthLayout.format = GL_DEPTH_COMPONENT;
	DepthLayout.type = GL_FLOAT;

	ShadowBuffer.setDrawAttachment(GL_NONE);
	ShadowBuffer.setLayout({ 2048, 2048, 0 });

	ShadowMap = ShadowBuffer.createDepthTextureAttachment(DepthLayout);
}

ShadowRenderer::~ShadowRenderer()
{

}

void ShadowRenderer::renderShadowMap(const Camera& camera, const Mesh& mesh, const Transform& transform, const Light& light, const Renderer& renderer)
{
	getShadowBuffer().bind();

	renderer.enableDepthTest();
	renderer.enableCullFace();
	renderer.cullBackFace();

	Matrix4 projection = light.getLightProjection(camera, transform);
	Matrix4 view = light.getLightView(camera, transform);
	Matrix4 model = transform.getWorldMatrix();

	Matrix4 depthMVP = projection * view * model;

	mShadowMapShader.start();

	mShadowMapShader.setUniform(mDepthMVPLocation, depthMVP);

	renderer.render(mesh.getVertexArray(), mesh.getIndexBuffer());

	mShadowMapShader.stop();

	renderer.disableCullFace();
	renderer.disableDepthTest();

	getShadowBuffer().unbind();
}

void ShadowRenderer::renderAdditiveShadow(const Camera& camera, const Mesh& mesh, const Transform& transform, const Light& light, const Renderer& renderer)
{
	// model WVP
	Matrix4 WorldViewProjection = camera.getProjectionMatrix() * camera.getViewMatrix() * transform.getWorldMatrix();

	// light WVP biased
	Matrix4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
	Matrix4 projection = light.getLightProjection(camera, transform);
	Matrix4 view = light.getLightView(camera, transform);
	Matrix4 model = transform.getWorldMatrix();
	Matrix4 depthMVP = projection * view * model;
	Matrix4 depthMVPBias = biasMatrix * depthMVP;

	// Bind depth texture
	getShadowMap().start();

	// Bind light intentisity, depth wvp biased and shadow map depth texture
	mShadowMapAdditiveShader.setUniform(mShadowIntensityLocation, 1 - light.intensity);
	mShadowMapAdditiveShader.setUniform(mDepthMVPBiasLocation, depthMVPBias);
	mShadowMapAdditiveShader.setUniform(mShadowMapLocation, (int)0);

	renderer.render(mesh.getVertexArray(), mesh.getIndexBuffer());

	getShadowMap().stop();
}

const FrameBuffer& ShadowRenderer::getShadowBuffer() const
{
	return ShadowBuffer;
}

const Texture2D& ShadowRenderer::getShadowMap() const
{
	return ShadowMap;
}

const FrameBuffer& ShadowRenderer::getFrameBuffer()
{
	return ShadowBuffer;
}
