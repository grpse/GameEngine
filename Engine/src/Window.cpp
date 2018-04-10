#include <GL/glew.h>
#include <iostream>
#include "Window.h"
#include "Input.h"
#include <GLFW/glfw3.h>


Window::Window()
{
}

void Window::start()
{
	/* Initialize the library */
	if (!glfwInit())
	{
		std::cout << "GLFW ERROR" << std::endl;
	}

	/* Create a windowed mode window and its OpenGL context */
	mWindow = glfwCreateWindow(mViewport.width, mViewport.height, "Commander", NULL, NULL);
	if (!mWindow)
	{
		std::cout << "Problem creating window" << std::endl;
		glfwTerminate();
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(mWindow);

	glewExperimental = GL_TRUE;
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		std::cout << "Glew Error: " << glewGetErrorString(error) << std::endl;
	}

	glfwSetKeyCallback(mWindow, Window::KeyCallback);
	glfwSetCursorPosCallback(mWindow, Window::MousePositionCallback);
	glfwSetMouseButtonCallback(mWindow, Window::MouseButtonCallback);
	glfwSetScrollCallback(mWindow, Window::ScrollCallback);
}


void Window::swapBuffers()
{
	glfwSwapBuffers(mWindow);
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
	glfwTerminate();
}

void Window::pollEvents()
{
	Input::clearInputs();
	glfwPollEvents();
}

bool Window::isOpen()
{
	return !glfwWindowShouldClose(mWindow);
}

void Window::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_REPEAT)
		Input::setPressedKeyRepeated(key);
	else if (action == GLFW_PRESS)
		Input::setPressedKey(key);
	else if (action == GLFW_RELEASE)
		Input::setReleasedKey(key);
}

void Window::MousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	Input::setMousePosition(xpos, ypos);

}

void Window::MouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
	if (action == GLFW_REPEAT)
		Input::setMouseButtonPressedRepeat(button);
	else if (action == GLFW_PRESS)
		Input::setMouseButtonPress(button);
	else if (action == GLFW_RELEASE)
		Input::setMouseButtonRelease(button);
}

void Window::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Input::setScrollOffset(xoffset, yoffset);
}
