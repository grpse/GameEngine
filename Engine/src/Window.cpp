#include <GL/glew.h>
#include <iostream>
#include "Window.h"
#include "Input.h"
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

bool Window::mShowTweak = false;
Rect lastViewport;

Window::Window()
	: mIsStarted(false)
{

}

Window::Window(const Window & other)
{
	mViewport = other.mViewport;
	mWindow = other.mWindow;
	mIsOpen = other.mIsOpen;
	mIsStarted = other.mIsStarted;
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

	lastViewport = mViewport;

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
	
	setupInputEventCallers();

	// Setup ImGui binding
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(mWindow, false);

	// Setup style
	ImGui::StyleColorsDark();
	//TODO: reflect display refresh rate to swap buffers delayed time
	//glfwSwapInterval(1);

	mIsStarted = true;
}

void Window::GUIFrame()
{
	ImGui_ImplGlfwGL3_NewFrame();
}

void Window::swapBuffers()
{
	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

	int width, height;
	glfwGetWindowSize(mWindow, &width, &height);
	mViewport.height = height;
	mViewport.width = width;

	setViewport(mViewport);

	glfwSwapBuffers(mWindow);
}

void Window::setViewport(Rect viewport)
{
	glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
	mViewport = viewport;
}

Rect& Window::getViewport()
{
	return mViewport;
}

void Window::finish()
{
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
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

void Window::CharCallback(GLFWwindow * window, uint codepoint, int mods)
{
	// TODO: implement	
}

void Window::setupInputEventCallers()
{
	glfwSetKeyCallback(mWindow, Window::KeyCallback);
	glfwSetCursorPosCallback(mWindow, Window::MousePositionCallback);
	glfwSetMouseButtonCallback(mWindow, Window::MouseButtonCallback);
	glfwSetScrollCallback(mWindow, Window::ScrollCallback);
	glfwSetCharModsCallback(mWindow, Window::CharCallback);
}