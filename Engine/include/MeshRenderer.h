#pragma once
#include <GL/glew.h>
#include "Renderer.h"
#include "LinearMath.h"
#include "Mesh.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "GLErrorHandling.h"
#include "MeshShaderSource.h"
#include "Transform.h"

class MeshRenderer{

public:

	MeshRenderer()
	{
		mShader.useVertexAttribute();
		mShader.useNormalAttribute();
		mShader.useTextureCoord0Attribute();

		mShader.useProjectionMatrix();
		mShader.useWorldViewMatrix();
		mShader.useWorldMatrix();
		mShader.useViewMatrix();

		mShader.buildShadersFromSource(MeshShaderSource);
	}

	~MeshRenderer()
	{

	}

	void setProjection(const Matrix4& projection)
	{
		mProjection = projection;
	}

    void render(const Camera& camera, const Mesh& mesh, const Transform& transform, const Renderer& renderer)
    {
		prepare(renderer);

		const auto& vao = mesh.getVertexArray();
		const auto& ibo = mesh.getIndexBuffer();

		const Matrix4& World = transform.getWorldMatrix();
		const Matrix4& View = camera.getViewMatrix();
		const Matrix4 WorldView = View * World;

		mShader.setWorldViewMatrix(WorldView);
		mShader.setWorldMatrix(World);
		mShader.setViewMatrix(View);
		
		renderer.render(vao, ibo);

		finishRendering(renderer);
    }

private:
	Matrix4 mProjection;
	ShaderProgram mShader;

	void prepare(const Renderer& renderer)
	{
		mShader.start();
		mShader.setProjectionMatrix(mProjection);
		renderer.enableDepthTest();
		renderer.enableCullFace();
		renderer.cullBackFace();
	}

	void finishRendering(const Renderer& renderer)
	{
		renderer.disableCullFace();
		renderer.disableDepthTest();
		mShader.stop();
	}
};