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

char ShadowShader[] = R"(

#BEGIN VERTEXSHADER

uniform mat4 depthMVP;

void main() {
	gl_Position = depthMVP * vec4(VertexPosition_ModelSpace, 1);
}

#END VERTEXSHADER

#BEGIN FRAGMENTSHADER

out float fragmentDepth;

void main() {
	fragmentDepth = gl_FragCoord.z;
}

#END FRAGMENTSHADER

)";

char ShadowMapShaderAdditive[] = R"(

#BEGIN VERTEXSHADER

uniform mat4 DepthMVPBias;

out vec4 ShadowCoord;

void main()
{
	gl_Position = WorldViewProjection * vec4(VertexPosition_ModelSpace, 1);	
	ShadowCoord = DepthMVPBias * vec4(VertexPosition_ModelSpace, 1);
}

#END VERTEXSHADER

#BEGIN FRAGMENTSHADER

uniform float ShadowIntensity;
uniform sampler2D ShadowMap;

in vec4 ShadowCoord;

void main() 
{
	float visibility = 1.0;
	float bias = 0.005;

	if (texture(ShadowMap, ShadowCoord.xy).z  <  ShadowCoord.z - bias) {
		visibility = 0.1;
	}
	
	gl_FragColor = vec4(1, 1, 1, 1) * visibility * ShadowIntensity;
}

#END FRAGMENTSHADER

)";




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

ShadowRenderer::ShadowRenderer(const ShadowRenderer& other)
{
	memcpy(this, &other, sizeof(ShadowRenderer));
}

ShadowRenderer::~ShadowRenderer()
{

}

void ShadowRenderer::renderShadowMap(const Camera& camera, const Mesh& mesh, const Transform& transform, const Light& light, const Renderer& renderer)
{
	ShadowBuffer.bind();

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

	ShadowBuffer.unbind();
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
	Matrix4 projection = Matrix4(1);// light.getLightProjection(camera, transform);
	Matrix4 view = Matrix4(1);// light.getLightView(camera, transform);
	Matrix4 model = transform.getWorldMatrix();
	Matrix4 depthMVP = projection * view * model;
	Matrix4 depthMVPBias = biasMatrix * depthMVP;

	// Bind depth texture
	getShadowMap().start();

	// Bind light intentisity, depth wvp biased and shadow map depth texture
	mShadowMapAdditiveShader.setUniform(mShadowIntensityLocation, 1 - light.intensity);
	mShadowMapAdditiveShader.setUniform(mDepthMVPBiasLocation, depthMVPBias);
	mShadowMapAdditiveShader.setUniform(mShadowMapLocation, (uint)0);

	renderer.render(mesh.getVertexArray(), mesh.getIndexBuffer());

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