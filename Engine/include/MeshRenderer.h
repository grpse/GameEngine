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
#include "Light.h"

class MeshRenderer{

public:

	MeshRenderer()
	{
		mShader.useVertexAttribute();
		mShader.useNormalAttribute();
		mShader.useTextureCoord0Attribute();

		mShader.useProjectionMatrix();
		mShader.useWorldViewProjectionMatrix();
		mShader.useWorldViewMatrix();
		mShader.useWorldMatrix();
		mShader.useViewMatrix();

		mShader.buildShadersFromSource(MeshShaderSource);

		mShader.start();
		mLightUniforms.position = mShader.getUniformLocation("directional.position");
		mLightUniforms.direction = mShader.getUniformLocation("directional.direction");
		mLightUniforms.color = mShader.getUniformLocation("directional.color");
		mLightUniforms.intensity = mShader.getUniformLocation("directional.intensity");
		mShader.stop();
	}

	~MeshRenderer()
	{

	}
	
    void render(const Camera& camera, const Mesh& mesh, const Transform& transform, const Light& directional, const Renderer& renderer)
    {
		prepare(renderer);

		const auto& vao = mesh.getVertexArray();
		const auto& ibo = mesh.getIndexBuffer();

		const Matrix4& World = transform.getWorldMatrix();
		const Matrix4& View = camera.getViewMatrix();
		const Matrix4& Projection = camera.getProjectionMatrix();
		const Matrix4 WorldView = View * World;
		const Matrix4 WorldViewProjection = Projection * WorldView;

		mShader.setProjectionMatrix(Projection);
		mShader.setWorldViewMatrix(WorldView);
		mShader.setWorldViewProjectionMatrix(WorldViewProjection);
		mShader.setWorldMatrix(World);
		mShader.setViewMatrix(View);
		mShader.setUniform(mLightUniforms.position, directional.position);
		mShader.setUniform(mLightUniforms.direction, directional.direction);
		mShader.setUniform(mLightUniforms.color, directional.color);
		mShader.setUniform(mLightUniforms.intensity, directional.intensity);
				
		renderer.render(vao, ibo);

		finishRendering(renderer);
    }


private:
	ShaderProgram mShader;	


	struct LightUniforms {
		uint position;
		uint direction;
		uint color;
		uint intensity;
	};

	LightUniforms mLightUniforms;

	void prepare(const Renderer& renderer)
	{
		mShader.start();
		renderer.enableDepthTest();
		//renderer.enableCullFace();
		//renderer.cullBackFace();
	}

	void finishRendering(const Renderer& renderer)
	{
		// renderer.disableCullFace();
		renderer.disableDepthTest();
		mShader.stop();
	}
};
