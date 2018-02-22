#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include "Window.h"
#include "Input.h"

Window::Window()
{
}

void Window::start()
{
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
		std::cout << "SDL2 Error: " << SDL_GetError() << std::endl;
	}

	mWindow = SDL_CreateWindow(
		"ParticleSystem",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		mViewport.width,
		mViewport.height,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
	);

	mIsOpen = true;
	
	SDL_GLContext context = SDL_GL_CreateContext(mWindow);

	glewExperimental = GL_TRUE;
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		std::cout << "Glew Error: " << glewGetErrorString(error) << std::endl;
	}
}

void Window::swapBuffers()
{
	SDL_GL_SwapWindow(mWindow);
}

void Window::setViewport(Rect viewport)
{
	glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
	mViewport = viewport;
}

Rect Window::getViewport()
{
	return mViewport;
}

void Window::finish()
{
	// Close and destroy the window
	SDL_DestroyWindow(mWindow);

	// Clean up
	SDL_Quit();
}

void Window::pollEvents()
{
	Input::clearInputs();
	SDL_SetRelativeMouseMode(SDL_FALSE);
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				mIsOpen = false;
				break;

			case SDL_KEYDOWN:
				Input::setPressedKey(event.key.keysym.scancode);
				break;

			case SDL_MOUSEMOTION: 
				{
					int x, y;
					SDL_GetRelativeMouseState(&x, &y);
					for (auto& mousemovelistener : mMouseMoveListeners) {
						mousemovelistener(x, y);
					}
				}
				
				break;

			default:
				break;
		}
	}
}

bool Window::isOpen()
{
	return mIsOpen;
}


void Window::onKeydown(const std::function<void(uint key)>& keydownListener)
{
	mKeydownListeners.push_back(keydownListener);
}

void Window::onMouseMove(const std::function<void(int x, int y)>& mousemoveListener)
{
	mMouseMoveListeners.push_back(mousemoveListener);
}