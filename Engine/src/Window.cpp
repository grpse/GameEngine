#include <GL/glew.h>
#include <iostream>
#include "Window.h"
#include "Input.h"
#include <GLFW/glfw3.h>
#include <AntTweakBar.h>

bool Window::mShowTweak = false;

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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

	// Initialize AntTweakBar
	TwInit(TW_OPENGL_CORE, NULL);

	setupInputEventCallers();

	//TODO: reflect display refresh rate to swap buffers delayed time
	//glfwSwapInterval(1);
}


void Window::swapBuffers()
{
	if (mShowTweak) TwDraw();
	glfwSwapBuffers(mWindow);
}

void Window::setViewport(Rect viewport)
{
	glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
	mViewport = viewport;
	TwWindowSize(viewport.width, viewport.height);
}

Rect Window::getViewport()
{
	return mViewport;
}

void Window::finish()
{
	TwTerminate();
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

void Window::setTitle(const std::string & title)
{
	const char* titlePtr = title.c_str();
	glfwSetWindowTitle(mWindow, titlePtr);
}

void Window::toogleTweakBar()
{
	mShowTweak = !mShowTweak;
}

void* Window::createTweak(const char* title)
{
	return (void*)TwNewBar(title);
}

void Window::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (mShowTweak)
	{
		TwEventKeyGLFW(key, action);
	}
	else 
	{
		if (action == GLFW_REPEAT)
			Input::setPressedKeyRepeated(key);
		else if (action == GLFW_PRESS)
			Input::setPressedKey(key);
		else if (action == GLFW_RELEASE)
			Input::setReleasedKey(key);
	}
}

void Window::MousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (mShowTweak)
	{
		TwEventMousePosGLFW(xpos, ypos);
	}
	else
	{
		Input::setMousePosition(xpos, ypos);
	}
}

void Window::MouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
	if (mShowTweak)
	{
		TwEventMouseButtonGLFW(button, action);
	}
	else
	{
		if (action == GLFW_REPEAT)
			Input::setMouseButtonPressedRepeat(button);
		else if (action == GLFW_PRESS)
			Input::setMouseButtonPress(button);
		else if (action == GLFW_RELEASE)
			Input::setMouseButtonRelease(button);
	}
}

void Window::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (mShowTweak)
	{
		TwEventMouseWheelGLFW(xoffset);
	}
	else
	{
		Input::setScrollOffset(xoffset, yoffset);
	}
}

void Window::CharCallback(GLFWwindow * window, uint codepoint, int mods)
{
	
	if (mShowTweak)
	{
		TwEventCharGLFW(codepoint, 0);
	}
	else 
	{
		//TODO: something else
	}

}

void Window::setupInputEventCallers()
{
	glfwSetKeyCallback(mWindow, Window::KeyCallback);
	glfwSetCursorPosCallback(mWindow, Window::MousePositionCallback);
	glfwSetMouseButtonCallback(mWindow, Window::MouseButtonCallback);
	glfwSetScrollCallback(mWindow, Window::ScrollCallback);
	glfwSetCharModsCallback(mWindow, Window::CharCallback);
}