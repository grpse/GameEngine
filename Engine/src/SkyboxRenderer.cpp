#include "GLErrorHandling.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "SkyboxRenderer.h"
#include "AttributesNamesDefines.h"
#include "SkyboxShaderSource.h"


float points[] = {
	-10.0f,  10.0f, -10.0f,
	-10.0f, -10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,
	10.0f,  10.0f, -10.0f,
	-10.0f,  10.0f, -10.0f,

	-10.0f, -10.0f,  10.0f,
	-10.0f, -10.0f, -10.0f,
	-10.0f,  10.0f, -10.0f,
	-10.0f,  10.0f, -10.0f,
	-10.0f,  10.0f,  10.0f,
	-10.0f, -10.0f,  10.0f,

	10.0f, -10.0f, -10.0f,
	10.0f, -10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f,  10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,

	-10.0f, -10.0f,  10.0f,
	-10.0f,  10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f, -10.0f,  10.0f,
	-10.0f, -10.0f,  10.0f,

	-10.0f,  10.0f, -10.0f,
	10.0f,  10.0f, -10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	-10.0f,  10.0f,  10.0f,
	-10.0f,  10.0f, -10.0f,

	-10.0f, -10.0f, -10.0f,
	-10.0f, -10.0f,  10.0f,
	10.0f, -10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,
	-10.0f, -10.0f,  10.0f,
	10.0f, -10.0f,  10.0f
};

SkyboxRenderer::SkyboxRenderer()
{
	VertexBufferLayout layout;
	layout.pushFloat(3, POSITION);

	mVAO.createVertexBuffer<float>(points, sizeof(points)/sizeof(float), layout);
}

SkyboxRenderer::SkyboxRenderer(const SkyboxRenderer & other)
{
	memcpy(this, &other, sizeof(SkyboxRenderer));
}

void SkyboxRenderer::setShader(const ShaderProgram & shader)
{
	mShader = shader;
}

void SkyboxRenderer::render(const Camera& camera, const CubeMap& SkyMap, const Renderer& renderer) const
{
	Matrix4 WVP = camera.getProjectionMatrix() * camera.getViewMatrix() * Matrix4(10.0);
	mShader.bind();
	mShader.setUniform("Skycube", SkyMap);
	mShader.setCameraPosition(camera.transform.getLocalPosition());
	mShader.setWorldViewProjectionMatrix(WVP);
	renderer.render(mVAO, 0, 36);
}