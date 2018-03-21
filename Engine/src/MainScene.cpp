#include "MeshRenderer.h"
#include "MainScene.h"
#include "Input.h"
#include "Actor.h"
#include "Loader.h"
#include "Window.h"
#include "Transform.h"
#include "Mesh.h"
#include "Light.h"

void MainScene::start()
{
	// Create suzanne actor to show
	Actor suzanne;
	suzanne.getTransform().setLocalPosition(Vector3(0, -1, 0));
	suzanne.getTransform().setLocalScale(Vector3(1, 1, 1));
	suzanne.getTransform().setLocalRotation(Vector3(0, 0, 0));
	suzanne.setRenderable<MeshRenderer>();
	Mesh suzanneGeometry = Loader::loadSimpleMesh("suzanne.obj");
	suzanne.getRenderable<MeshRenderer>()->setMesh(suzanneGeometry);
	suzanne.getRenderable<MeshRenderer>()->setCastShadow(true);

	// create floor to show
	Actor floor;
	floor.getTransform().setLocalPosition(Vector3(0, -2, 0));
	floor.getTransform().setLocalScale(Vector3(5, 5, 5));
	floor.getTransform().setLocalRotation(Quaternion(1, 0, 0, 3.1415));

	Mesh floorGeometry = Mesh::createQuad();
	floor.setRenderable<MeshRenderer>();
	floor.getRenderable<MeshRenderer>()->setMesh(floorGeometry);
	floor.getRenderable<MeshRenderer>()->setReceiveShadow(true);

	// Create main camera to this scene
	Rect viewport = Window::getInstance().getViewport();

	Camera::Format cameraFormat;
	cameraFormat.fieldOfView = 45.0f;
	cameraFormat.aspectRatio = viewport.width / viewport.height;
	cameraFormat.nearPlane = 0.001f;
	cameraFormat.farPlane = 10000.0;

	mCamera.setFormat(cameraFormat, Camera::Type::Perspective);
	mCamera.getTransform().setLocalPosition(Vector3(0, 0, 10));
	mCamera.getTransform().setLocalRotation(Quaternion(0, 1, 0, 3.1415));

	//get current camera position and rotation
	mCameraPosition = mCamera.getTransform().getLocalPosition();
	mCameraRotation = mCamera.getTransform().getLocalRotation();

	// Create directional light
	Light directional;
	directional.intensity = 0.5f;
	directional.type = Light::Type::Directional;
	directional.direction = Vector3(-1.0f, -1.0f, -1.0f);

	// push to the scene
	addChild(mCamera);
	addChild(directional);
	addChild(floor);
	addChild(suzanne);

	// set mouse movement handler
	Window::getInstance().onMouseMove([&](int x, int y) {

		float rotationDiff = (float)(10.0 * Time::getDeltaTime());

		if (x > 0 || x < 0) {
			mCameraRotation.y += x * rotationDiff;
		}

		if (y > 0 || y < 0) {
			const float halfPi = Math::radians(90.0);
			float nextRotationOnX = mCameraRotation.x - y * rotationDiff;
			mCameraRotation.x = Math::clamp(nextRotationOnX, -halfPi, halfPi);
		}

		mCamera.getTransform().setLocalRotation(Quaternion(mCameraRotation));
	});
}

void MainScene::update(float deltaTime)
{
	float moveDiff = (float)(80.0 * deltaTime);

	Vector3 front = mCamera.getTransform().getFront();
	Vector3 right = mCamera.getTransform().getRight();

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

	mCamera.getTransform().setLocalPosition(mCameraPosition);
}