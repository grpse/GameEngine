#include <GL/glew.h>
#include "GameLoop.h"
#include "Time.h"
#include "MainScene.h"
#include "Window.h"
#include <iostream>
#include "ServiceLocator.h"
#include "GLErrorHandling.h"
#include "ShadowRenderer.h"

void GameLoop::start()
{
	Locator::provideWindow(new Window);

	Locator::locateWindow()->setViewport({ 0, 0, 640, 480 });
	Locator::locateWindow()->start();

	Time::start();
	
	Scene* currentScene = new MainScene;
	// particle system setup
	//Texture2D particleTexture = Loader::loadRGBATexture2D("start.png");
	//ParticleSystem mParticleSystem(50.0, 25, 1, 4);
	//mParticleSystem.loadTexture(particleTexture);
	
	currentScene->setup();
	currentScene->start();

	Locator::provideShadowRenderer(new ShadowRenderer);

	while (Locator::locateWindow()->isOpen()) {
		float dt = (float)Time::getDeltaTime();
		
		//mParticleSystem.emitParticle(Vector3(0, -5, 0));
		//mParticleSystem.update();

		currentScene->updateComponents(dt);

		Locator::locateWindow()->swapBuffers();
		Locator::locateWindow()->pollEvents();

		Time::updateDeltaTime();

		std::cout << "Delta Time: " << Time::getDeltaTime() << "ms" << std::endl;
	}

	Locator::locateWindow()->finish();
}