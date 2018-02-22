#pragma once
#include "Camera.h"
#include "Typedefs.h"
#include "LinearMath.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Transform.h"
#include "ShaderProgram.h"
#include "DirectionalLight.h"

const char* ShadowShader = R"(

#BEGIN VERTEXSHADER

uniform mat4 depthMVP;

void main() {
	gl_Position = depthMVP * vec4(VertexPosition_ModelSpace, 1);
}

#END VERTEXSHADER

#BEGIN FRAGMENTSHADER

out float fragmentDepth;

void main() {
	fragmentDepth = gl_FragCoord.z;
}

#END FRAGMENTSHADER

)";

class ShadowRenderer {

public:
	ShadowRenderer()
	{
		mShader.useVertexAttribute();
		mShader.buildShadersFromSource(ShadowShader);

		mShader.start();
		mDepthMVPLocation = mShader.getUniformLocation("depthMVP");
		mShader.stop();
	}

	void render(const Camera& camera, const Mesh& mesh, const Transform& transform, const DirectionalLight& directional, const Renderer& renderer)
	{
		renderer.enableDepthTest();
		//renderer.enableCullFace();
		//renderer.cullBackFace();

		Matrix4 projection = Math::ortho(-2.0, 2.0, -2.0, 2.0, 0.0001, 10.0);
		Matrix4 view = Math::lookAt(-directional.direction, directional.direction, Vector3(0, 1, 0));
		Matrix4 model = transform.getWorldMatrix();
		
		Matrix4 depthMVP =  projection * view * model;
		
		mShader.start();

		mShader.setUniform(mDepthMVPLocation, depthMVP);

		renderer.render(mesh.getVertexArray(), mesh.getIndexBuffer());

		mShader.stop();

		//renderer.cullBackFace();
		renderer.disableDepthTest();
	}

private:
	ShaderProgram mShader;
	uint mDepthMVPLocation;
};