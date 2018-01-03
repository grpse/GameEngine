#include <GL/glew.h>
#include <SDL.h>
#include <iostream>
#include "LinearMath.h"
#include "GameLoop.h"
#include "Time.h"
#include "ParticleSystem.h"
#include "Camera.h"
#include "ParticleMaster.h"
#include "GLErrorHandling.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Loader.h"
#include <SOIL.h>

#define RC_INVOKED 1

Uint64 NOW = 0;
Uint64 LAST = 0;
double deltaTimeInSecondsFraction = 0;
double mElapsedSecond = 0;

Window mWindow;
Camera mCamera;
Matrix4 mProjectionMatrix;

void update();

const uint ArrowUp = 82;
const uint ArrowDown = 81;
const uint ArrowLeft = 80;
const uint ArrowRight = 79;
const uint Space = 44;
const uint W = 26;
const uint S = 22;
const uint D = 7;
const uint A = 4;

uint mBuffer;


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
	GLCall(glCullFace(GL_FRONT));
	GLCall(glClearColor(0, 0, .0, 1));

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	GLCall(glGenBuffers(1, &mBuffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, mBuffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW));

	mCamera.transform.setLocalPosition({ 0, 0, 10 });

	Texture2D particleTexture = Loader::loadRGBATexture2D("start.png");

	ParticleSystem mParticleSystem(50.0, 25, 1, 4);	
	mParticleSystem.setProjectionMatrix(mProjectionMatrix);
	mParticleSystem.loadTexture(particleTexture);

	Vector3 position = mCamera.transform.getLocalPosition();
	Vector3 rotation = Math::eulerAngles(mCamera.transform.getLocalRotation()) * 3.14159f / 180.f;

	mWindow.onKeydown([&](uint key) {
		
		std::cout << "Foi " << key << std::endl;
	
		float moveDiff = (float)(50.0 * Time::getDeltaTime());
		float rotationDiff = (float) (50.0 * Time::getDeltaTime());
		Vector3 front = mCamera.transform.getFront();
		Vector3 right = mCamera.transform.getRight();
		switch(key) {
			case ArrowUp:
				position -= Math::normalize(front) * moveDiff;
				break;

			case ArrowDown:
				position += Math::normalize(front) * moveDiff;
				break;

			case ArrowRight:
				position -= Math::normalize(right) * moveDiff;
				break;

			case ArrowLeft:
				position += Math::normalize(right) * moveDiff;
				break;

			case W:
				rotation.x += rotationDiff;
				break;

			case S:
				rotation.x -= rotationDiff;
				break;

			case A:
				rotation.y += rotationDiff;
				break;

			case D:
				rotation.y -= rotationDiff;
				break;
		}
		
		mCamera.transform.setLocalPosition(position);
		mCamera.transform.setLocalRotation(Quaternion(rotation));
		
		mCamera.printPositionAndTarget();

		// if (Space == key) {
		// 	mParticleMaster.instantiateOne({ 0, 0, 0 }, Vector3(0, 20, 0), 10, 1, 5, 1);
		// }
		
		std::cout << "Rotation Euler: " << Math::to_string(rotation) << std::endl;

		// UPDATE FRONT AND RIGHT VECTORS
		mCamera.transform.getWorldMatrix();
	});



	mWindow.onMouseMove([&](int x, int y) {

		// Compute new orientation
		// horizontalAngle += mouseSpeed * float(viewport.width / 2.0f - x);
		// verticalAngle += mouseSpeed * float(viewport.height / 2.0f - y);

		// Vector3 direction(
		// 	cos(verticalAngle) * sin(horizontalAngle),
		// 	sin(verticalAngle),
		// 	cos(verticalAngle) * cos(horizontalAngle)
		// );

		// mCamera.transform.setLocalRotation(Quaternion(direction));
		// mCamera.transform.getWorldMatrix();
	});

	while (mWindow.isOpen()) {
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));	
		
		mParticleSystem.emitParticle(Vector3(0, 0, 0));
		mParticleSystem.update();
		mParticleSystem.render(mCamera);
		
		update();

		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTimeInSecondsFraction = (double)((NOW - LAST) * 1000 / SDL_GetPerformanceFrequency()) / 1000;
		Time::setDeltaTime(deltaTimeInSecondsFraction);
	}

	mWindow.finish();
}

void update()
{
	//3d stuff


	// 2d stuff

	mWindow.swapBuffers();
	mWindow.pollEvents();
}


const float GameLoop::Gravity()
{
	return -9.8f;
}
