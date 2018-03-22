#pragma once
#include "Typedefs.h"
#include "LinearMath.h"
#include "ShaderProgram.h"
#include "OSExport.h"
#include "Texture2D.h"
#include "FrameBuffer.h"

class Camera;
class Light;
class Transform;
class Mesh;
class Renderer;

class ShadowRenderer {

public:
	ShadowRenderer();
	~ShadowRenderer();
	void renderShadowMap(const Camera& camera, const Mesh& mesh, const Transform& transform, const Light& light, const Renderer& renderer);
	void renderAdditiveShadow(const Camera& camera, const Mesh& mesh, const Transform& transform, const Light& light, const Renderer& renderer);
	const FrameBuffer& getShadowBuffer() const;
	const Texture2D& getShadowMap() const;
	const FrameBuffer& getFrameBuffer();

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