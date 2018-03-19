#pragma once
#include "Typedefs.h"

class Renderer;
class Camera;
class Actor;
class Light;
class Material;

class Renderable {

public:

	enum class QueueType {
		Opaque, Cutoff, Transparent, Count
	};

	virtual QueueType getRenderQueue() const = 0;
	virtual void preRender(const Camera& camera, const Light* lights, uint lightsCount, const Actor& actor, const Renderer& renderer) const = 0;
	virtual void render(const Camera& camera, const Light* lights, uint lightsCount, const Actor& actor, const Renderer& renderer) const = 0;
	virtual void postRender(const Camera& camera, const Light* lights, uint lightsCount, const Actor& actor, const Renderer& renderer) const = 0;
	virtual void setMaterial(const Material& material) = 0;
	virtual const Material& getMaterial() const = 0;
	virtual bool hasPrePassStep() const = 0;
	virtual bool hasPostPassStep() const = 0;
};