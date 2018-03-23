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
#include "DirectionalLight.h"
#include "Light.h"

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

		mShader.start();
		mShadowMapLocation = mShader.getUniformLocation("ShadowMap");
		mDepthMVPBiasLocation = mShader.getUniformLocation("DepthMVPBias");
		mLightDirectionLocation = mShader.getUniformLocation("LightDirection");
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
		const Matrix4 WorldView = View * World;

		mShader.setProjectionMatrix(camera.getProjectionMatrix());
		mShader.setWorldViewMatrix(WorldView);
		mShader.setWorldMatrix(World);
		mShader.setViewMatrix(View);
		mShader.setUniform(mLightDirectionLocation, directional.direction);
				
		renderer.render(vao, ibo);

		finishRendering(renderer);
    }

	void render(const Camera& camera, const Mesh& mesh, const Transform& transform, const Light& directional, const Texture2D& depthTexture, const Renderer& renderer)
	{
		prepare(renderer);

		const auto& vao = mesh.getVertexArray();
		const auto& ibo = mesh.getIndexBuffer();

		const Matrix4& World = transform.getWorldMatrix();
		const Matrix4& View = camera.getViewMatrix();
		const Matrix4 WorldView = View * World;

		mShader.setProjectionMatrix(camera.getProjectionMatrix());
		mShader.setWorldViewMatrix(WorldView);
		mShader.setWorldMatrix(World);
		mShader.setViewMatrix(View);

		// light stuff
		Matrix4 projection = Math::ortho(-2.0, 2.0, -2.0, 2.0, 0.0001, 10.0);
		Matrix4 view = Math::lookAt(-directional.direction, directional.direction, Vector3(0, 1, 0));
		Matrix4 model = transform.getWorldMatrix();

		Matrix4 biasMatrix(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
		);

		Matrix4 depthMVP = projection * view * model;
		Matrix4 depthMVPBias = biasMatrix * depthMVP;

		depthTexture.start();

		mShader.setUniform(mDepthMVPBiasLocation, depthMVPBias);
		mShader.setUniform(mShadowMapLocation, (uint)0);
		mShader.setUniform(mLightDirectionLocation, directional.direction);

		renderer.render(vao, ibo);

		depthTexture.stop();
		finishRendering(renderer);
	}

private:
	ShaderProgram mShader;
	uint mShadowMapLocation;
	uint mDepthMVPBiasLocation;
	uint mLightDirectionLocation;

	void prepare(const Renderer& renderer)
	{
		mShader.start();
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