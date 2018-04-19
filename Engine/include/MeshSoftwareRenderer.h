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
		mShader.addProgram(MeshSoftwareShaderSource);
	}

	~MeshSoftwareRenderer()
	{

	}

	void setup()
	{
		mShader.bind();
		mLightUniforms.position = mShader.getUniformLocation("directional_software.position");
		mLightUniforms.direction = mShader.getUniformLocation("directional_software.direction");
		mLightUniforms.color = mShader.getUniformLocation("directional_software.color");
		mLightUniforms.intensity = mShader.getUniformLocation("directional_software.intensity");
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

		mTempVertices.assign(mesh.getVertices().begin(), mesh.getVertices().end());

		//multiply
		multiplyByMatrix4AndDivideByW(mTempVertices, WorldViewProjection);
		mesh.getVertexArray().updateBuffer(0, mTempVertices.data(), mTempVertices.size() * sizeof(Vertex));
		renderer.render(mesh);

		finishRendering(renderer);
	}


private:
	ShaderProgram mShader;

	std::vector<Vertex> mTempVertices;


	struct LightUniforms {
		uint position;
		uint direction;
		uint color;
		uint intensity;
	};

	LightUniforms mLightUniforms;

	void multiplyByMatrix4AndDivideByW(std::vector<Vertex>& vertices, const Matrix4& m)
	{
		for (auto& vertice : vertices)
		{
			Vector4 v = (m * Vector4(vertice.position, 1));
			vertice.position = -Vector3(v.x, v.y, v.z) / v.w;
		}
	}

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
