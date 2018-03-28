#pragma once

#include "Mesh.h"
#include "Material.h"
#include "Renderable.h"

class Camera;
class Light;
class Actor;
class Renderer;
class ShadowRenderer;

class MeshRenderer : public Renderable {

public:

	MeshRenderer();
	~MeshRenderer();

	void setMesh(const Mesh& mesh);
	void setReceiveShadow(const bool shouldReceiveShadow);
	void setCastShadow(const bool shouldCastShadow);
	virtual void preRender(const Camera& camera, const Light* lights, uint lightsCount, const Actor& actor, const Renderer& renderer) const;
	virtual void render(const Camera& camera, const Light* lights, uint lightsCount, const Actor& actor, const Renderer& renderer) const;
	virtual void postRender(const Camera& camera, const Light* lights, uint lightsCount, const Actor& actor, const Renderer& renderer) const;

	Renderable::QueueType getRenderQueue() const;
	virtual void setMaterial(const Material & material);
	virtual const Material & getMaterial() const;
	virtual bool hasPrePassStep() const;
	virtual bool hasPostPassStep() const;

private:

	uint mShadowMapLocation;
	uint mDepthMVPBiasLocation;
	uint mDirectional_directionLocation;
	uint mDirectional_colorLocation;
	uint mDirectional_intensityLocation;
	ShadowRenderer* mShadowRenderer;

	bool mShouldCastShadow;
	bool mShouldReceiveShadow;

	Mesh mCurrentMesh;
	mutable Material mMaterial;

	void prepare(const Renderer& renderer) const;
	void finishRendering(const Renderer& renderer) const;
};