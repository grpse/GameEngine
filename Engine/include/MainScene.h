#pragma once

#include "Scene.h"
#include "Camera.h"
#include "LinearMath.h"
#include "Light.h"
#include "Actor.h"

class MainScene : public Scene {

public:
	void start();
	void update(float deltaTime);

private:
	Actor mFloor;
	Actor mSuzanne;
	Light mDirectional;
	Camera mCamera;
	Vector3 mCameraPosition;
	Quaternion mCameraRotation;
};