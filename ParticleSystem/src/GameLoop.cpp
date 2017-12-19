#include <SDL.h>
#include "LinearMath.h"
#include "GameLoop.h"
#include "Time.h"
#include "ParticleSystem.h"

Uint64 NOW = 0;
Uint64 LAST = 0;
double deltaTime = 0;
Window mWindow;

const float GRAVITY = -9.8f;


void GameLoop::start()
{
	NOW = SDL_GetPerformanceCounter();

	mWindow.setViewport({ 0, 0, 640, 480 });
	mWindow.start();

	while (mWindow.isOpen()) {
		update();
	}

	mWindow.finish();
}

void GameLoop::update()
{
	LAST = NOW;
	NOW = SDL_GetPerformanceCounter();

	deltaTime = (double)((NOW - LAST) * 1000 / SDL_GetPerformanceFrequency());
	Time::setDeltaTime(deltaTime);

	// RENDERING



	mWindow.swapBuffers();
	mWindow.pollEvents();
}

const float GameLoop::Gravity()
{
	return GRAVITY;
}
