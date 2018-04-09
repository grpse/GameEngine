#pragma once
#include "Typedefs.h"
#define NUMBER_OF_KEYS 256

#include <GLFW/glfw3.h>

const uint ArrowUp = GLFW_KEY_UP;
const uint ArrowDown = GLFW_KEY_DOWN;
const uint ArrowLeft = GLFW_KEY_LEFT;
const uint ArrowRight = GLFW_KEY_RIGHT;
const uint Space = GLFW_KEY_SPACE;
const uint W = GLFW_KEY_W;
const uint S = GLFW_KEY_S;
const uint D = GLFW_KEY_D;
const uint A = GLFW_KEY_A;
const uint MouseLeftButton = GLFW_MOUSE_BUTTON_LEFT;
const uint MouseRightButton = GLFW_MOUSE_BUTTON_RIGHT;
class Window;

class Input {
public:
	static bool isPressedKey(uint key);
	static void getMousePosition(double& x, double& y);
	static void getMouseDelta(double& x, double& y);
	static bool isMouseButtonPressedRepeat(uint mouseButton);
	static bool isMouseButtonPress(uint mouseButton);
	static bool isMouseButtonRelease(uint mouseButton);

private:
	static void clearInputs();
	static void setPressedKey(uint key);
	static void setMousePosition(double x, double y);
	static void setMouseButtonPressedRepeat(uint mouseButton);
	static void setMouseButtonPress(uint mouseButton);
	static void setMouseButtonRelease(uint mouseButton);

	friend class Window;
};