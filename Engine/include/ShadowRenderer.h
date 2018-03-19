#pragma once
#include "Typedefs.h"
#include "LinearMath.h"
#include "ShaderProgram.h"

class Camera;
class FrameBuffer;
class Light;
class Transform;
class Mesh;
class Renderer;
struct Texture2DLayout;
class Texture2D;

class ShadowRenderer {

public:
	ShadowRenderer();
	void renderShadowMap(const Camera& camera, const Mesh& mesh, const Transform& transform, const Light& light, const Renderer& renderer);
	void renderAdditiveShadow(const Camera& camera, const Mesh& mesh, const Transform& transform, const Light& light, const Renderer& renderer);
	static void initializeSharedResources();
	const FrameBuffer& getShadowBuffer() const;
	const Texture2D& getShadowMap() const;
	static const FrameBuffer& getFrameBuffer();

private:
	ShaderProgram mShadowMapShader;
	ShaderProgram mShadowMapAdditiveShader;
	uint mDepthMVPLocation;
	uint mDepthMVPBiasLocation;
	uint mShadowIntensityLocation;
	uint mShadowMapLocation;

};