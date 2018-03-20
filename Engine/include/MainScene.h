#pragma once

#include "Scene.h"
#include "Camera.h"
#include "LinearMath.h"

class MainScene : public Scene {

public:
	void start();
	void update(float deltaTime);

private:
	Camera mCamera;
	Vector3 mCameraPosition;
	Quaternion mCameraRotation;
};