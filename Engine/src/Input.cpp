#include "Input.h"
#include <string.h>

#define NUMBER_OF_MOUSE_BUTTONS (GLFW_MOUSE_BUTTON_LAST + 1)

bool mPressedKeys[NUMBER_OF_KEYS];
bool mMouseButtonPressedRepeat[NUMBER_OF_MOUSE_BUTTONS];
bool mMouseButtonPress[NUMBER_OF_MOUSE_BUTTONS];
bool mMouseButtonRelease[NUMBER_OF_MOUSE_BUTTONS];

double mX = 0;
double mY = 0;

double mDeltaX = 0;
double mDeltaY = 0;

void Input::clearInputs()
{
	memset(mPressedKeys, false, NUMBER_OF_KEYS);
	memset(mMouseButtonPressedRepeat, false, NUMBER_OF_MOUSE_BUTTONS);
	memset(mMouseButtonPress, false, NUMBER_OF_MOUSE_BUTTONS);
	memset(mMouseButtonRelease, false, NUMBER_OF_MOUSE_BUTTONS);
	mDeltaX = 0;
	mDeltaY = 0;
}

void Input::setPressedKey(uint key)
{
	mPressedKeys[key] = true;
}

bool Input::isPressedKey(uint key)
{
	return mPressedKeys[key];
}

void Input::getMousePosition(double& x, double& y)
{
	x = mX;
	y = mY;
}

void Input::getMouseDelta(double& x, double& y)
{
	x = mDeltaX;
	y = mDeltaY;
}

bool Input::isMouseButtonPressedRepeat(uint mouseButton)
{
	return mMouseButtonPressedRepeat[mouseButton];
}

bool Input::isMouseButtonPress(uint mouseButton)
{
	return mMouseButtonPress[mouseButton];
}

bool Input::isMouseButtonRelease(uint mouseButton)
{
	return mMouseButtonRelease[mouseButton] = true;
}

void Input::setMousePosition(double x, double y)
{
	mDeltaX = x - mX;
	mDeltaY = y - mY;

	mX = x;
	mY = y;
}

void Input::setMouseButtonPressedRepeat(uint mouseButton)
{
	mMouseButtonPressedRepeat[mouseButton] = true;
}

void Input::setMouseButtonPress(uint mouseButton)
{
	mMouseButtonPress[mouseButton] = true;
}

void Input::setMouseButtonRelease(uint mouseButton)
{
	mMouseButtonRelease[mouseButton] = true;
}