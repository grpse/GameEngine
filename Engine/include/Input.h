#pragma once
#include "Typedefs.h"
#include "OSExport.h"
#define NUMBER_OF_KEYS 256

//#include <GLFW/glfw3.h>

const uint ArrowUp = 265;//  GLFW_KEY_UP;
const uint ArrowDown = 264;// GLFW_KEY_DOWN;
const uint ArrowLeft = 263;// GLFW_KEY_LEFT;
const uint ArrowRight = 262;// GLFW_KEY_RIGHT;
const uint Space = 32; // GLFW_KEY_SPACE;
const uint W = 87;// GLFW_KEY_W;
const uint S = 83; // GLFW_KEY_S;
const uint D = 68; //GLFW_KEY_D;
const uint A = 65;// GLFW_KEY_A;
const uint C = 67; // GLFW_KEY_C;
const uint MouseLeftButton = 0; // GLFW_MOUSE_BUTTON_LEFT;
const uint MouseRightButton = 1;// GLFW_MOUSE_BUTTON_RIGHT;
const uint F = 70;// GLFW_KEY_F;
const uint M = 77;// GLFW_KEY_M;
const uint R = 82; // GLFW_KEY_R;

class Window;

class ENGINE_API Input {
public:
	static bool isPressedKeyRepeated(uint key);
	static bool wasPressedKey(uint key);
	static bool wasReleasedKey(uint key);
	static void getMousePosition(double& x, double& y);
	static void getMouseDelta(double& x, double& y);
	static bool isMouseButtonPressedRepeat(uint mouseButton);
	static bool isMouseButtonPress(uint mouseButton);
	static bool isMouseButtonRelease(uint mouseButton);
	static void getScrollOffsetDelta(double& x, double& y);

private:
	static void clearInputs();
	static void setPressedKeyRepeated(uint key);
	static void setPressedKey(uint key);
	static void setReleasedKey(uint key);

	static void setMousePosition(double x, double y);
	static void setMouseButtonPressedRepeat(uint mouseButton);
	static void setMouseButtonPress(uint mouseButton);
	static void setMouseButtonRelease(uint mouseButton);
	static void setScrollOffset(double x, double y);
	friend class Window;
};