#include "MeshRenderer.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Camera.h"
#include "GLErrorHandling.h"
#include "MeshShaderSource.h"
#include "Transform.h"
#include "Light.h"

MeshRenderer::MeshRenderer()
{

	mPhong.addProgram(PhongShading);
	mGouraudAD.addProgram(GouraudAD);
	mGouraudADS.addProgram(GouraudADS);

	mShader = mPhong;
}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::setup()	
{
	mShader.bind();
	mLightUniforms.position = mShader.getUniformLocation("light.position");
	mLightUniforms.direction = mShader.getUniformLocation("light.direction");
	mLightUniforms.color = mShader.getUniformLocation("light.color");
	mLightUniforms.intensity = mShader.getUniformLocation("light.intensity");
	mDiffuseColorLocation = mShader.getUniformLocation("DiffuseColor");
}


void MeshRenderer::render(const Camera& camera, const Mesh& mesh, const Transform& transform, const Light& directional, const Renderer& renderer)
{
	prepare(renderer);

	const Matrix4& NormalMatrix = transform.getWorldInverseTranspose();
	const Matrix4& World = transform.getWorldMatrix();
	const Matrix4& View = camera.getViewMatrix();
	const Matrix4& Projection = camera.getProjectionMatrix();
	const Matrix4 WorldView = View * World;
	const Matrix4 WorldViewProjection = Projection * WorldView;

	mShader.setWorldMatrix(World);
	mShader.setViewMatrix(View);
	mShader.setWorldViewMatrix(WorldView);
	mShader.setWorldViewProjectionMatrix(WorldViewProjection);
	mShader.setWorldInverseTranspose(NormalMatrix);

	mShader.setUniform(mLightUniforms.position, directional.position);
	mShader.setUniform(mLightUniforms.direction, directional.direction);
	mShader.setUniform(mLightUniforms.color, directional.color);
	mShader.setUniform(mLightUniforms.intensity, directional.intensity);

	renderer.render(mesh);

	finishRendering(renderer);
}

void MeshRenderer::setDiffuseColor(const Color32& color)
{
	mShader.setUniform(mDiffuseColorLocation, color);
}

ShaderProgram& MeshRenderer::getShaderProgram()
{
	return mShader;
}

void MeshRenderer::usePhong()
{
	mShader = mPhong;
}

void MeshRenderer::useGouraudAD()
{
	mShader = mGouraudAD;
}

void MeshRenderer::useGouraudADS()
{
	mShader = mGouraudADS;
}


/// PRIVATE
void MeshRenderer::prepare(const Renderer& renderer)
{
	mShader.bind();
	renderer.enableDepthTest();
	renderer.setDepthMask();
	renderer.enableCullFace();
	renderer.cullBackFace();
}

void MeshRenderer::finishRendering(const Renderer& renderer)
{
	renderer.disableCullFace();
	renderer.disableDepthTest();
}