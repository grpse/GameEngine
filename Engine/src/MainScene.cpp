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
	//mCamera.transform.setLocalRotation(Quaternion(0, 1, 0, 3.1415));

	//get current camera position and rotation
	mCameraPosition = mCamera.transform.getLocalPosition();
	mCameraRotation = mCamera.transform.getLocalRotation();

	// Create directional light
	mDirectional.intensity = 1.0f;
	mDirectional.type = Light::Type::Directional;
	mDirectional.direction = Vector3(-1.0f, -1.0f, -1.0f);
	mDirectional.color = Color32(1, 1, 1, 1);
	mDirectional.position = Math::one<Vector3>();

	// push to the scene
	addChild(mCamera);
	addChild(mDirectional);
	addChild(mFloor);
	addChild(mSuzanne);

	// set mouse movement handler
	Camera* ptrCam = &mCamera;
	Quaternion quatCam = mCameraRotation;
	Locator::locateWindow()->onMouseMove([ptrCam, &quatCam](int x, int y) -> void {

		float rotationDiff = (float)(10.0 * Time::getDeltaTime());

		if (x > 0 || x < 0) {
			quatCam.y += x * rotationDiff;
		}

		if (y > 0 || y < 0) {
			const float halfPi = Math::radians(90.0);
			float nextRotationOnX = quatCam.x - y * rotationDiff;
			quatCam.x = Math::clamp(nextRotationOnX, -halfPi, halfPi);
		}

		ptrCam->transform.setLocalRotation(Quaternion(quatCam));
		ptrCam->transform.getWorldMatrix();


	});
}

void MainScene::update(float deltaTime)
{
	float moveDiff = 80.0f * deltaTime;

	Vector3 front = mCamera.transform.getFront();
	Vector3 right = mCamera.transform.getRight();

	if (Input::isPressedKey(ArrowUp)) 
	{
		mCameraPosition += Math::normalize(front) * moveDiff;
	}

	if (Input::isPressedKey(ArrowDown)) 
	{
		mCameraPosition -= Math::normalize(front) * moveDiff;
	}

	if (Input::isPressedKey(ArrowRight)) 
	{
		mCameraPosition += Math::normalize(right) * moveDiff;
	}

	if (Input::isPressedKey(ArrowLeft)) 
	{
		mCameraPosition -= Math::normalize(right) * moveDiff;
	}

	mCamera.transform.setLocalPosition(mCameraPosition);
	mCamera.transform.getWorldMatrix();
}