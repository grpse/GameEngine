#pragma once
#include <functional>
#include <vector>
#include "Rect.h"
#include "OSExport.h"
#include "Typedefs.h"

struct GLFWwindow;
class ENGINE_API Window {

public:
	Window();
	void start();
	void swapBuffers();
	void setViewport(Rect viewport);
	Rect getViewport();
	void finish();
	void pollEvents();
	bool isOpen();
	
private:
	GLFWwindow* mWindow;
	Rect mViewport;

	bool mIsOpen;

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MousePositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};