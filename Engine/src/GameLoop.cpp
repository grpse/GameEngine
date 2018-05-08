#include <GL/glew.h>
#include <cassert>
#include "GameLoop.h"
#include "Time.h"
#include "Scene.h"
#include "Window.h"

GameLoop* GameLoop::mInstance = nullptr;

GameLoop::GameLoop()
	: mWindowStarted(true)
{
	mInstance = this;
}

void GameLoop::start()
{
	// Don't open program if not started window
	assert(mWindow.isStarted() && "SHOULD SET VIEWPORT AND START THE WINDOW FIRST");
		
	mCurrentScene->start();

	while (mWindow.isOpen()) {

		mWindow.GUIFrame();

		mCurrentScene->update((float)Time::getDeltaTime());
		mCurrentScene->render();
		mCurrentScene->onGUI();
		
		mWindow.pollEvents();
		Time::updateDeltaTime();
		mWindow.swapBuffers();
	}

	mWindow.finish();
}

void GameLoop::setScene(Scene* scene)
{
	mCurrentScene = scene;
}

GameLoop& GameLoop::getInstance()
{
	return *mInstance;
}