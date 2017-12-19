#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include "Window.h"

Window::Window()
{
}

void Window::start()
{
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
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				mIsOpen = false;
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
