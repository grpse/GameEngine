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
	Window(const Window& other);
	void start();
	void swapBuffers();
	void setViewport(Rect viewport);
	Rect getViewport();
	void finish();
	void pollEvents();
	bool isOpen();
	void setTitle(const std::string& title);
	void toogleTweakBar();
	void* createTweak(const char* title);

	void GUIFrame();
	inline bool isStarted() { return mIsStarted; }
	
private:
	GLFWwindow* mWindow;
	Rect mViewport;

	bool mIsOpen;
	bool mIsStarted;
	static bool mShowTweak;

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MousePositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void CharCallback(GLFWwindow* window, uint codepoint, int mods);

	void setupInputEventCallers();
};