#include "GLErrorHandling.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "SkyboxRenderer.h"
#include "AttributesNamesDefines.h"

const char SkyboxShader[] = R"(

#queue Opaque

#vertex vertProgram
#fragment fragProgram

#begin uniforms
uniform vec3 CameraPosition;
uniform samplerCube Skycube;
#end uniforms

#begin vertex_variables
out vec3 texcoords;
#end vertex_variables

Vector4 vertProgram() {
  texcoords = POSITION;
  return WORLDVIEWPROJECTION * vec4(POSITION + CameraPosition, 1.0);
}

#begin fragment_variables
in vec3 texcoords;
#end fragment_variables

Vector4 fragProgram() {
	return texture(Skycube, texcoords);
}

)";

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
	mShader.addProgram(SkyboxShader);


	VertexBuffer vbo;
	VertexBufferLayout layout;

	vbo.bind();
	vbo.load(points, sizeof(points));
	layout.pushFloat(3, POSITION);

	mVAO.generateBuffer();
	mVAO.addVertexBuffer(vbo, layout);
}

void SkyboxRenderer::setCubeMap(const CubeMap& cubeMap)
{
	mCubeMap = cubeMap;
}

void SkyboxRenderer::setup()
{
	mShader.bind();
	mCubeMapLocation = mShader.getUniformLocation("Skycube");
	mCameraPositionLocation = mShader.getUniformLocation("CameraPosition");
}

void SkyboxRenderer::render(const Camera& camera, const Renderer& renderer) const
{
	renderer.setDepthMask();
	renderer.disableDepthTest();
	
	Matrix4 WVP = camera.getProjectionMatrix() * camera.getViewMatrix() * Matrix4(10.0);

	mShader.bind();
	mShader.setUniform(mCubeMapLocation, mCubeMap);
	mShader.setUniform(mCameraPositionLocation, camera.transform.getLocalPosition());
	mShader.setWorldViewProjectionMatrix(WVP);
	renderer.render(mVAO, 0, 36);
	renderer.enableDepthTest();
	renderer.unsetDepthMask();

}