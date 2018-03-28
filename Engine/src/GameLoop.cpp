#include <GL/glew.h>
#include <SDL.h>
#include <iostream>
#include "LinearMath.h"
#include "GameLoop.h"
#include "Time.h"
#include "ParticleSystem.h"
#include "Camera.h"
#include "GLErrorHandling.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Loader.h"
#include <SOIL.h>
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Renderer.h"
#include "ParticleRenderer.h"
#include "FrameBuffer.h"
#include "BillboardRenderer.h"
#include "ShadowRenderer.h"
#include "Input.h"
#include "Light.h"

Uint64 NOW = 0;
Uint64 LAST = 0;
double deltaTimeInSecondsFraction = 0;
double mElapsedSecond = 0;

Window mWindow;
Camera mCamera;
Matrix4 mProjectionMatrix;



static Vector3 quadVertices[] = {
	Vector3(-0.5, -0.5, 0),
	Vector3( 0.5, -0.5, 0),
	Vector3( 0.5, 0.5, 0),
	Vector3(-0.5, 0.5, 0),
};

void GameLoop::start()
{

	Rect viewport = { 0, 0, 640, 480 };
	mWindow.setViewport(viewport);
	mWindow.start();

	Camera orthoCam;

	Camera::Format miniCam;
	miniCam.viewport = { 0, 0, 2, 2 };
	miniCam.nearPlane = 0.001f;
	miniCam.farPlane = 10000.0f;

	orthoCam.setFormat(miniCam, Camera::Type::Orthographic);

	Camera::Format cameraFormat;
	cameraFormat.fieldOfView = 45.0f;
	cameraFormat.aspectRatio = viewport.width / viewport.height;
	cameraFormat.nearPlane = 0.001f;
	cameraFormat.farPlane = 10000.0;

	mCamera.setFormat(cameraFormat, Camera::Type::Perspective);

	NOW = SDL_GetPerformanceCounter();

	mCamera.transform.setLocalPosition({ 0, 2, -10 });
	mCamera.transform.setLocalRotation(Vector3(3.1415f/2.0f, 3.1415f, 0));

	// particle system setup
	 Texture2D particleTexture = Loader::loadRGBATexture2D("start.png");
	// ParticleSystem mParticleSystem(50.0, 25, 1, 4);
	// mParticleSystem.loadTexture(particleTexture);


	Vector3 position = mCamera.transform.getLocalPosition();
	Vector3 rotation = Math::eulerAngles(mCamera.transform.getLocalRotation()) * 3.14159f / 180.f;

	auto updateCameraPositionAndRotation = [&]() {

		float moveDiff = (float)(80.0 * Time::getDeltaTime());
		float rotationDiff = (float)(20.0 * Time::getDeltaTime());

		Vector3 front = mCamera.transform.getFront();
		Vector3 right = mCamera.transform.getRight();
		if (Input::isPressedKey(ArrowUp)) {
			position += Math::normalize(front) * moveDiff;
		}

		if (Input::isPressedKey(ArrowDown)) {
			position -= Math::normalize(front) * moveDiff;
		}

		if (Input::isPressedKey(ArrowRight)) {
			position += Math::normalize(right) * moveDiff;
		}

		if (Input::isPressedKey(ArrowLeft)) {
			position -= Math::normalize(right) * moveDiff;
		}

		if (Input::isPressedKey(W)) {
			rotation.x += rotationDiff;
		}

		if (Input::isPressedKey(S)) {
			rotation.x -= rotationDiff;
		}

		if (Input::isPressedKey(A)) {
			rotation.y -= rotationDiff;
		}

		if (Input::isPressedKey(D)) {
			rotation.y += rotationDiff;
		}

		mCamera.transform.setLocalRotation(Quaternion(rotation));
		orthoCam.transform.setLocalRotation(Quaternion(rotation));
		mCamera.transform.setLocalPosition(position);
		orthoCam.transform.setLocalPosition(position);


		// UPDATE FRONT AND RIGHT VECTORS
		mCamera.transform.getWorldMatrix();
		orthoCam.transform.getWorldMatrix();
	};

	mWindow.onMouseMove([&](int x, int y) {

		float rotationDiff = (float)(10.0 * Time::getDeltaTime());

		if (x > 0 || x < 0) {
			rotation.y += x * rotationDiff;
		}

		if (y > 0 || y < 0) {
			const float halfPi = Math::radians(90.0);
			float nextRotationOnX = rotation.x - y * rotationDiff;
			rotation.x = Math::clamp(nextRotationOnX, -halfPi, halfPi);
		}

		mCamera.transform.setLocalRotation(Quaternion(rotation));
		orthoCam.transform.setLocalRotation(Quaternion(rotation));
	});
	
	//ParticleRenderer particleRenderer;
	MeshRenderer meshRenderer;

	Mesh suzanne = Loader::loadMesh("cow_up.in");
	Transform suzanneTransform;

	Mesh quad = Mesh::createQuad();
	Transform quadTransform;
	quadTransform.setLocalPosition({ 0, -2, 0 });
	quadTransform.setLocalScale({ 5, 5, 5 });
	quadTransform.setLocalRotation(Vector3(Math::radians(180.0), 0, 0));

	suzanneTransform.setLocalPosition({ 0, -1, 0 });
	suzanneTransform.setLocalScale({ 1, 1, 1 });
	suzanneTransform.setLocalRotation(Vector3(0, 0, 0));

	Renderer renderer;

	Texture2DLayout layout;
	layout.target = GL_TEXTURE_2D;
	layout.border = 0;
	layout.format = GL_RGB;
	layout.height = 512;
	layout.width = 512;
	layout.internalFormat = GL_RGB;
	layout.level = 0;
	layout.type = GL_UNSIGNED_BYTE;

	Texture2DLayout depthLayout;
	depthLayout.target = GL_TEXTURE_2D;
	depthLayout.level = 0;
	depthLayout.internalFormat = GL_DEPTH_COMPONENT16;
	depthLayout.width = 1024;
	depthLayout.height = 1024;
	depthLayout.border = 0;
	depthLayout.format = GL_DEPTH_COMPONENT;
	depthLayout.type = GL_FLOAT;


	//BillboardRenderer billboardRenderer;
	// ShadowRenderer shadowRenderer;


	Light directional;
	directional.direction = Vector3(-1, -1, -1);
	directional.color = Color32(1, 1, 1, 1);
	directional.intensity = 1;
	
	float angle = 0;
	float mx = 0;
	float mz = 0;
	
	while (mWindow.isOpen()) {
		
		float dt = (float)Time::getDeltaTime();
		angle += dt * 1;
		mx = cos(angle);
		mz = sin(angle);

		suzanneTransform.setLocalPosition({ mx, 0, mz });

		//mParticleSystem.emitParticle(Vector3(0, -5, 0));
		// mParticleSystem.update();

		// shadowRenderer.getShadowBuffer().setLayout({ 1024, 1024, 0 });
		// shadowRenderer.getShadowBuffer().bind();

		renderer.clearColorAndDepth();
		// TODO: in order to draw shadow map, we need a shadow renderer
		//			where geometries, lights and camera will create the
		//			depth buffer appropriated to use on shadow projection.
		// shadowRenderer.renderShadowMap(mCamera, suzanne, suzanneTransform, directional, renderer);
		// shadowRenderer.renderShadowMap(mCamera, quad, quadTransform, directional, renderer);

		// shadowRenderer.getFrameBuffer().unbind();

		//renderer.clearColorAndDepth();
		//renderer.setViewport(viewport);

		meshRenderer.render(mCamera, suzanne, suzanneTransform, directional, renderer);
		meshRenderer.render(mCamera, quad, quadTransform, directional, renderer);
		//meshRenderer.render(mCamera, suzanne, suzanneTransform, directional, shadowRenderer.getShadowMap(), renderer);
		//meshRenderer.render(mCamera, quad, quadTransform, directional, shadowRenderer.getShadowMap(), renderer);
		
		//particleRenderer.render(mParticleSystem, mCamera, renderer);


		// if (shadowRenderer.getShadowBuffer().isComplete())
		//		billboardRenderer.render(shadowRenderer.getShadowMap(), { 0.5f, -0.5f, 0.5f, 0.5f }, renderer);
		//billboardRenderer.render(particleTexture, { 0.5f, -0.5f, 0.5f, 0.5f }, renderer);

		mWindow.swapBuffers();
		mWindow.pollEvents();
		updateCameraPositionAndRotation();

		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTimeInSecondsFraction = (double)((NOW - LAST) * 1000 / SDL_GetPerformanceFrequency()) / 1000;
		Time::setDeltaTime(deltaTimeInSecondsFraction);		
	}

	mWindow.finish();
}