#include <GL/glew.h>
#include "ShadowRenderer.h"
#include "LinearMath.h"
#include "Camera.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Transform.h"
#include "ShaderProgram.h"
#include "Light.h"
#include "FrameBuffer.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "LinearMath.h"
#include "Mesh.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "Light.h"
#include "ShadowShader.h"

ShadowRenderer::ShadowRenderer()
{
	// Shadow map shader to generate depth texture
	mShadowMapShader.addProgram(ShadowShader);
	
	// Additive Shadow Shader to add Shadow to Scene
	mShadowMapAdditiveShader.addProgram(ShadowMapShaderAdditive);
	
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

ShadowRenderer::ShadowRenderer(const ShadowRenderer& other)
{
	memcpy(this, &other, sizeof(ShadowRenderer));
}

ShadowRenderer::~ShadowRenderer()
{

}

void ShadowRenderer::setup()
{
	mShadowMapShader.bind();
	mDepthMVPLocation = mShadowMapShader.getUniformLocation("depthMVP");

	mShadowMapAdditiveShader.bind();
	mDepthMVPBiasLocation = mShadowMapAdditiveShader.getUniformLocation("DepthMVPBias");
	mShadowIntensityLocation = mShadowMapAdditiveShader.getUniformLocation("ShadowIntensity");
	mShadowMapLocation = mShadowMapAdditiveShader.getUniformLocation("ShadowMap");
}

void ShadowRenderer::renderAdditiveShadow(const Camera& camera, const Mesh& mesh, const Transform& transform, const Light& light, const Renderer& renderer)
{
	renderer.enableDepthTest();
	renderer.enableCullFace();
	renderer.cullBackFace();
	renderer.enableBlend();
	renderer.setBlendSrcAlpha_OneMinusSrcColor();

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
	mShadowMapAdditiveShader.bind();
	mShadowMapAdditiveShader.setUniform(mShadowIntensityLocation, 1 - light.intensity);
	mShadowMapAdditiveShader.setUniform(mDepthMVPBiasLocation, depthMVPBias);
	mShadowMapAdditiveShader.setInteger(mShadowMapLocation, 0);

	renderer.render(mesh.getVertexArray(), mesh.getIndexBuffer());
	mShadowMapAdditiveShader.unbind();

	getShadowMap().stop();
}

FrameBuffer& ShadowRenderer::getShadowBuffer() 
{
	return ShadowBuffer;
}

const Texture2D& ShadowRenderer::getShadowMap() const
{
	return ShadowMap;
}

FrameBuffer& ShadowRenderer::getFrameBuffer()
{
	return ShadowBuffer;
}