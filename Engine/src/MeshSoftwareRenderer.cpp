#include <GL/glew.h>
#include "MeshSoftwareRenderer.h"
#include "MeshSoftwareRendererShader.h"
#include "Transform.h"
#include "Light.h"
#include "Mesh.h"
#include "Camera.h"
#include "Renderer.h"

std::vector<Vertex> mTempVertices;

MeshSoftwareRenderer::MeshSoftwareRenderer()
{

}

MeshSoftwareRenderer::~MeshSoftwareRenderer()
{

}

void MeshSoftwareRenderer::setShader(const ShaderProgram& shader)
{
	mShader = shader;
}

void MeshSoftwareRenderer::render(const Camera& camera, const Mesh& mesh, const Transform& transform, const Renderer& renderer) const
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

	mTempVertices.resize(mesh.getVertices().size());

	for (uint i = 0; i < mTempVertices.size(); i++)
	{
		mTempVertices[i] = mesh.getVertices()[i];
		Vector4 v = (WorldViewProjection * Vector4(mTempVertices[i].position, 1));
		Vector4 n = (NormalMatrix * Vector4(mTempVertices[i].normal, 1));
		mTempVertices[i].position = v / v.w;
		mTempVertices[i].normal = Math::normalize(n);
	}

	mesh.getVertexArray().updateBuffer(0, mTempVertices.data(), mTempVertices.size() * sizeof(Vertex));
	renderer.render(mesh);
}