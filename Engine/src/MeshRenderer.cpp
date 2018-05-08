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

}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::setShader(const ShaderProgram& shader)
{
	mShader = shader;
}

void MeshRenderer::render(const Camera& camera, const Mesh& mesh, const Transform& transform, const Renderer& renderer) const
{
	const Matrix4& NormalMatrix = transform.getWorldInverseTranspose();
	const Matrix4& World = transform.getWorldMatrix();
	const Matrix4& View = camera.getViewMatrix();
	const Matrix4& Projection = camera.getProjectionMatrix();
	const Matrix4 WorldView = View * World;
	const Matrix4 WorldViewProjection = Projection * WorldView;

	mShader.bind();
	mShader.setWorldMatrix(World);
	mShader.setViewMatrix(View);
	mShader.setWorldViewMatrix(WorldView);
	mShader.setWorldViewProjectionMatrix(WorldViewProjection);
	mShader.setWorldInverseTranspose(NormalMatrix);

	renderer.render(mesh);
}