#pragma once
#include <GL/glew.h>
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

		mShader.buildShadersFromSource(MeshShaderSource);
	}

	~MeshRenderer()
	{

	}

	void setProjection(const Matrix4& projection)
	{
		mProjection = projection;
	}

    void render(const Camera& camera, const Mesh& mesh, const Transform& transform)
    {
		prepare();
		const auto& vao = mesh.getVertexArray();
		const auto& ibo = mesh.getIndexBuffer();


		const Matrix4 WorldView = camera.createViewMatrix() * transform.getWorldMatrix();

		mShader.setWorldViewMatrix(WorldView);
		vao.bind();
		ibo.bind();

		GLCall(glDrawElements(GL_TRIANGLES, ibo.getElementCount(), GL_UNSIGNED_INT, (const void*)0));

		vao.unbind();
		finishRendering();
    }

private:
	Matrix4 mProjection;
	ShaderProgram mShader;

	void prepare()
	{
		mShader.start();
		mShader.setProjectionMatrix(mProjection);
		GLCall(glEnable(GL_BLEND));
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR));
	}

	void finishRendering()
	{
		GLCall(glDisable(GL_DEPTH_TEST));
		GLCall(glDisable(GL_BLEND));
		mShader.stop();
	}
};