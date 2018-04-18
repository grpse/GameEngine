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

		mShader.addProgram(MeshShaderSource);
	}

	~MeshRenderer()
	{

	}

	void setup()
	{
		mShader.bind();
		mLightUniforms.position = mShader.getUniformLocation("directional.position");
		mLightUniforms.direction = mShader.getUniformLocation("directional.direction");
		mLightUniforms.color = mShader.getUniformLocation("directional.color");
		mLightUniforms.intensity = mShader.getUniformLocation("directional.intensity");
	}
	
    void render(const Camera& camera, const Mesh& mesh, const Transform& transform, const Light& directional, const Renderer& renderer)
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
		
		mesh.render(renderer);

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
		mShader.bind();
		renderer.enableDepthTest();
		renderer.setDepthMask();
		renderer.enableCullFace();
		renderer.cullBackFace();
	}

	void finishRendering(const Renderer& renderer)
	{
		renderer.disableCullFace();
		renderer.disableDepthTest();
	}
};
