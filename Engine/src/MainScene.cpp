#include "MeshRenderer.h"
#include "MainScene.h"
#include "Input.h"
#include "Actor.h"
#include "Loader.h"
#include "Window.h"
#include "Transform.h"
#include "Mesh.h"
#include "Light.h"
#include "ServiceLocator.h"
#include <iostream>

void MainScene::start()
{
	// Create suzanne actor to show
	
	mSuzanne.transform.setLocalPosition(Vector3(0, -1, 0));
	mSuzanne.transform.setLocalScale(Vector3(1, 1, 1));
	mSuzanne.transform.setLocalRotation(Vector3(0, 0, 0));
	MeshRenderer* suzanneRenderer = new MeshRenderer;
	mSuzanne.setRenderable(suzanneRenderer);
	Mesh suzanneGeometry = Loader::loadSimpleMesh("suzanne.obj");
	suzanneRenderer->setMesh(suzanneGeometry);
	suzanneRenderer->setCastShadow(true);

	// create floor to show
	mFloor.transform.setLocalPosition(Vector3(0, -2, 0));
	mFloor.transform.setLocalScale(Vector3(5, 5, 5));
	mFloor.transform.setLocalRotation(Quaternion(1, 0, 0, 3.1415));

	Mesh floorGeometry = Mesh::createQuad();
	MeshRenderer* floorRenderer = new MeshRenderer;
	mFloor.setRenderable(floorRenderer);
	floorRenderer->setMesh(floorGeometry);
	floorRenderer->setReceiveShadow(true);

	// Create main camera to this scene
	Rect viewport = Locator::locateWindow()->getViewport();

	Camera::Format cameraFormat;
	cameraFormat.fieldOfView = 45.0f;
	cameraFormat.aspectRatio = viewport.width / viewport.height;
	cameraFormat.nearPlane = 0.001f;
	cameraFormat.farPlane = 10000.0;

	mCamera.setFormat(cameraFormat, Camera::Type::Perspective);
	mCamera.transform.setLocalPosition(Vector3(0, 0, 10));
	mCamera.transform.setLocalRotation(Quaternion(0, 1, 0, 3.1415));

	//get current camera position and rotation
	mCameraPosition = mCamera.transform.getLocalPosition();
	mCameraRotation = mCamera.transform.getLocalRotation();

	// Create directional light
	mDirectional.intensity = 0.5f;
	mDirectional.type = Light::Type::Directional;
	mDirectional.direction = Vector3(-1.0f, -1.0f, -1.0f);

	// push to the scene
	addChild(mCamera);
	addChild(mDirectional);
	addChild(mFloor);
	addChild(mSuzanne);

	// set mouse movement handler
	Locator::locateWindow()->onMouseMove([&](int x, int y) {

		float rotationDiff = (float)(10.0 * Time::getDeltaTime());

		if (x > 0 || x < 0) {
			mCameraRotation.y += x * rotationDiff;
		}

		if (y > 0 || y < 0) {
			const float halfPi = Math::radians(90.0);
			float nextRotationOnX = mCameraRotation.x - y * rotationDiff;
			mCameraRotation.x = Math::clamp(nextRotationOnX, -halfPi, halfPi);
		}

		mCamera.transform.setLocalRotation(Quaternion(mCameraRotation));
		std::cout << "Mouse Coords: " << x << ", " << y << std::endl;
	});
}

void MainScene::update(float deltaTime)
{
	float moveDiff = (float)(80.0 * deltaTime);

	Vector3 front = mCamera.transform.getFront();
	Vector3 right = mCamera.transform.getRight();

	if (Input::isPressedKey(ArrowUp)) {
		mCameraPosition += Math::normalize(front) * moveDiff;
	}

	if (Input::isPressedKey(ArrowDown)) {
		mCameraPosition -= Math::normalize(front) * moveDiff;
	}

	if (Input::isPressedKey(ArrowRight)) {
		mCameraPosition += Math::normalize(right) * moveDiff;
	}

	if (Input::isPressedKey(ArrowLeft)) {
		mCameraPosition -= Math::normalize(right) * moveDiff;
	}

	mCamera.transform.setLocalPosition(mCameraPosition);
}