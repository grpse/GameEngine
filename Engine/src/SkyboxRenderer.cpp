#include "GLErrorHandling.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "SkyboxRenderer.h"
#include "AttributesNamesDefines.h"

const char SkyboxShader[] = R"(

#queue Opaque

#begin vertexshader

uniform vec3 CameraPosition;

out vec3 texcoords;

void main() {
  texcoords = POSITION;
  gl_Position = WORLDVIEWPROJECTION * vec4(POSITION + CameraPosition, 1.0);
}
#end vertexshader

#begin fragmentshader
in vec3 texcoords;
uniform samplerCube CubeMap;
out vec4 frag_colour;

void main() {
  frag_colour = texture(CubeMap, texcoords);
}
#end fragmentshader

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
	mShader.buildShadersFromSource(SkyboxShader);
	mShader.bind();
	mCubeMapLocation = mShader.getUniformLocation("CubeMap");
	mCameraPositionLocation = mShader.getUniformLocation("CameraPosition");
	mShader.unbind();

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