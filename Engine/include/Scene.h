#pragma once
#include <vector>

class Renderer;
class Light;
class Camera;
class Actor;

class Scene {

public:

	void setup();

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
	Renderer* mRenderer;
};