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

Uint64 NOW = 0;
Uint64 LAST = 0;
double deltaTimeInSecondsFraction = 0;
double mElapsedSecond = 0;

Window mWindow;
Camera mCamera;
Matrix4 mProjectionMatrix;

const uint ArrowUp = 82;
const uint ArrowDown = 81;
const uint ArrowLeft = 80;
const uint ArrowRight = 79;
const uint Space = 44;
const uint W = 26;
const uint S = 22;
const uint D = 7;
const uint A = 4;

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

	float fov = 45.0f;
	float aspectRatio = viewport.width / viewport.height;
	float nearPlane = 0.001f;
	float farPlane = 10000.0;
	mProjectionMatrix = Math::perspective(fov, aspectRatio, nearPlane, farPlane);

	NOW = SDL_GetPerformanceCounter();

	//glFrontFace(GL_CCW);
	GLCall(glEnable(GL_DEPTH_TEST));
	//GLCall(glEnable(GL_CULL_FACE));
	//GLCall(glCullFace(GL_FRONT));
	GLCall(glClearColor(0, 0, .0, 1));

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	mCamera.transform.setLocalPosition({ 0, 0, 10 });

	// particle system setup
	Texture2D particleTexture = Loader::loadRGBATexture2D("start.png");
	ParticleSystem mParticleSystem(50.0, 25, 1, 4);
	mParticleSystem.loadTexture(particleTexture);


	Vector3 position = mCamera.transform.getLocalPosition();
	Vector3 rotation = Math::eulerAngles(mCamera.transform.getLocalRotation()) * 3.14159f / 180.f;

	mWindow.onKeydown([&](uint key) {
		
		std::cout << "Foi " << key << std::endl;
	
		float moveDiff = (float)(50.0 * Time::getDeltaTime());
		float rotationDiff = (float) (1.0 * Time::getDeltaTime());
		Vector3 front = mCamera.transform.getFront();
		Vector3 right = mCamera.transform.getRight();
		if (key == ArrowUp) {
			position += Math::normalize(front) * moveDiff;
		}

		if (key == ArrowDown) {
			position -= Math::normalize(front) * moveDiff;
		}

		if (key == ArrowRight) {
			position += Math::normalize(right) * moveDiff;
		}
		
		if (key == ArrowLeft) {
			position -= Math::normalize(right) * moveDiff;
		}
	
		if (key == W) {
			rotation.x += rotationDiff;
		}

		if (key == S) {
			rotation.x -= rotationDiff;
		}

		if (key == A) {
			rotation.y += rotationDiff;
		}

		if (key == D) {
			rotation.y -= rotationDiff;
		}
		
		mCamera.transform.setLocalPosition(position);
		mCamera.transform.setLocalRotation(Quaternion(rotation));

		// UPDATE FRONT AND RIGHT VECTORS
		mCamera.transform.getWorldMatrix();
	});

	ParticleRenderer particleRenderer;
	particleRenderer.setProjectionMatrix(mProjectionMatrix);

	MeshRenderer meshRenderer;
	meshRenderer.setProjection(mProjectionMatrix);
	Mesh suzanne = Loader::loadSimpleMesh("suzanne.obj");
	Transform suzanneTransform;

	Mesh quad = Mesh::createQuad();
	Transform quadTransform;
	quadTransform.setLocalPosition({ 0, -5, 0 });
	quadTransform.setLocalScale({ 10, 10, 10 });
	quadTransform.setLocalRotation(Vector3(Math::radians(180.0), 0, 0));

	suzanneTransform.setLocalPosition({ 0, 0, 0 });
	suzanneTransform.setLocalScale({ 1, 1, 1 });
	suzanneTransform.setLocalRotation(Vector3(0, 0, 0));

	Renderer renderer;
	
	while (mWindow.isOpen()) {
		renderer.clearColorAndDepth();
		
		mParticleSystem.emitParticle(Vector3(0, -5, 0));
		mParticleSystem.update();

		meshRenderer.render(mCamera, suzanne, suzanneTransform, renderer);
		meshRenderer.render(mCamera, quad, quadTransform, renderer);

		particleRenderer.render(mParticleSystem, mCamera, renderer);
		
		mWindow.swapBuffers();
		mWindow.pollEvents();

		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTimeInSecondsFraction = (double)((NOW - LAST) * 1000 / SDL_GetPerformanceFrequency()) / 1000;
		Time::setDeltaTime(deltaTimeInSecondsFraction);
	}

	mWindow.finish();
}

const float GameLoop::Gravity()
{
	return -9.8f;
}
