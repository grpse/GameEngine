#include "GLErrorHandling.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "SkyboxRenderer.h"

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
	mShader.start();
	mCubeMapLocation = mShader.getUniformLocation("CubeMap");
	mCameraPositionLocation = mShader.getUniformLocation("CameraPosition");
	mShader.stop();

	VertexBuffer vbo;
	VertexBufferLayout layout;

	vbo.bind();
	vbo.load(points, sizeof(points));
	layout.pushFloat(3);

	mVAO.generateBuffer();
	mVAO.bind();
	mVAO.setVertexBuffer(vbo, layout);
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

	mShader.start();
	mShader.setUniform(mCubeMapLocation, mCubeMap);
	mShader.setUniform(mCameraPositionLocation, camera.transform.getLocalPosition());
	mShader.setWorldViewProjectionMatrix(WVP);
	renderer.render(mVAO, 0, 36);
	renderer.enableDepthTest();
	renderer.unsetDepthMask();

}