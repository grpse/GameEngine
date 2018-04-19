#pragma once
#include <GL/glew.h>
#include "Renderer.h"
#include "LinearMath.h"
#include "Mesh.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "MeshSoftwareRendererShader.h"
#include "Transform.h"
#include "Light.h"
#include <algorithm>

class MeshSoftwareRenderer {

public:

	MeshSoftwareRenderer()
	{
		mPhong.addProgram(PhongSoftwareShading);
		mGouraudAD.addProgram(GouraudSoftwareADShading);
		mGouraudADS.addProgram(GouraudSoftwareADSShading);

		mShader = mPhong;
	}

	~MeshSoftwareRenderer()
	{

	}

	void setup()
	{
		mShader.bind();
		mLightUniforms.position = mShader.getUniformLocation("light.position");
		mLightUniforms.direction = mShader.getUniformLocation("light.direction");
		mLightUniforms.color = mShader.getUniformLocation("light.color");
		mLightUniforms.intensity = mShader.getUniformLocation("light.intensity");
		mDiffuseColorLocation = mShader.getUniformLocation("DiffuseColor");

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

		finishRendering(renderer);
	}

	void setDiffuseColor(Color32 color)
	{
		mShader.setUniform(mDiffuseColorLocation, color);
	}

	void usePhong()
	{
		mShader = mPhong;
	}

	void useGouraudAD()
	{
		mShader = mGouraudAD;
	}

	void useGouraudADS()
	{
		mShader = mGouraudADS;
	}

	inline ShaderProgram& getShaderProgram()
	{
		return mShader;
	}

private:
	ShaderProgram mShader;
	ShaderProgram mPhong;
	ShaderProgram mGouraudAD;
	ShaderProgram mGouraudADS;

	std::vector<Vertex> mTempVertices;


	struct LightUniforms {
		uint position;
		uint direction;
		uint color;
		uint intensity;
	};

	uint mDiffuseColorLocation;

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
