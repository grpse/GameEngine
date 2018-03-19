#pragma once

#include <vector>
#include <utility>

#include "Actor.h"
#include "Camera.h"
#include "Light.h"
#include "Renderer.h"

class Scene {

public:

	virtual void start() = 0;
	virtual void update(float deltaTime) = 0;

	void addChild(const Actor& actor);
	void addChild(const Camera& camera);
	void addChild(const Light& light);
	void updateComponents(float deltaTime);

private:
	std::vector<Actor*> mActors;
	std::vector<Camera*> mCameras;
	std::vector<Light*> mLights;
	Renderer mRenderer;
};