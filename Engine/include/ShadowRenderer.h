#pragma once
#include "Typedefs.h"
#include "LinearMath.h"
#include "ShaderProgram.h"
#include "OSExport.h"
#include "Texture2D.h"
#include "FrameBuffer.h"
#include "Camera.h"
#include "Light.h"
#include "Transform.h"
#include "Mesh.h"
#include "Renderer.h"

class ShadowRenderer {

public:
	ShadowRenderer();
	ShadowRenderer(const ShadowRenderer& other);
	~ShadowRenderer();
	void renderShadowMap(const Camera& camera, const Mesh& mesh, const Transform& transform, const Light& light, const Renderer& renderer)
	{
		ShadowBuffer.bind();

		renderer.enableDepthTest();
		renderer.enableCullFace();
		renderer.cullBackFace();

		Matrix4 projection = light.getLightProjection(camera, transform);
		Matrix4 view = light.getLightView(camera, transform);
		Matrix4 model = transform.getWorldMatrix();

		Matrix4 depthMVP = projection * view * model;

		mShadowMapShader.bind();

		mShadowMapShader.setUniform(mDepthMVPLocation, depthMVP);

		renderer.render(mesh.getVertexArray(), mesh.getIndexBuffer());

		mShadowMapShader.unbind();

		renderer.disableCullFace();
		renderer.disableDepthTest();

		ShadowBuffer.unbind();
	}
	void renderAdditiveShadow(const Camera& camera, const Mesh& mesh, const Transform& transform, const Light& light, const Renderer& renderer);
	FrameBuffer& getShadowBuffer();
	const Texture2D& getShadowMap() const;
	FrameBuffer& getFrameBuffer();

private:
	ShaderProgram mShadowMapShader;
	ShaderProgram mShadowMapAdditiveShader;
	uint mDepthMVPLocation;
	uint mDepthMVPBiasLocation;
	uint mShadowIntensityLocation;
	uint mShadowMapLocation;

	Texture2DLayout DepthLayout;
	FrameBuffer ShadowBuffer;
	Texture2D ShadowMap;

};