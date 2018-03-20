#include <GL/glew.h>
#include <SDL.h>
#include <iostream>
#include "GameLoop.h"
#include "Time.h"
#include "ParticleSystem.h"
#include "Camera.h"
#include "GLErrorHandling.h"
#include "Loader.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Input.h"
#include "Light.h"
#include "Actor.h"
#include "Renderable.h"
#include "Scene.h"
#include "LinearMath.h"
#include "Transform.h"
#include "Input.h"
#include "Window.h"
#include "MainScene.h"


void GameLoop::start()
{
	Uint64 NOW = 0;
	Uint64 LAST = 0;
	double deltaTimeInSecondsFraction = 0;
	double mElapsedSecond = 0;
	NOW = SDL_GetPerformanceCounter();

	Rect viewport = { 0, 0, 640, 480 };
	Window mWindow;
	mWindow.setViewport(viewport);
	mWindow.start();
	
	Scene* currentScene = new MainScene;
	// particle system setup
	//Texture2D particleTexture = Loader::loadRGBATexture2D("start.png");
	//ParticleSystem mParticleSystem(50.0, 25, 1, 4);
	//mParticleSystem.loadTexture(particleTexture);
		
	currentScene->start();

	while (mWindow.isOpen()) {
		float dt = (float)Time::getDeltaTime();
		
		//mParticleSystem.emitParticle(Vector3(0, -5, 0));
		//mParticleSystem.update();

		currentScene->updateComponents(dt);

		mWindow.swapBuffers();
		mWindow.pollEvents();

		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTimeInSecondsFraction = (double)((NOW - LAST) * 1000 / SDL_GetPerformanceFrequency()) / 1000;
		Time::setDeltaTime(deltaTimeInSecondsFraction);
	}

	mWindow.finish();
}