#include "Input.h"
#include <string.h>
#include <GLFW/glfw3.h>

#define NUMBER_OF_MOUSE_BUTTONS (GLFW_MOUSE_BUTTON_LAST + 1)

bool mPressedKeysRepeated[NUMBER_OF_KEYS];
bool mPressedKeys[NUMBER_OF_KEYS];
bool mReleasedKeys[NUMBER_OF_KEYS];
bool mMouseButtonPressedRepeat[NUMBER_OF_MOUSE_BUTTONS];
bool mMouseButtonPress[NUMBER_OF_MOUSE_BUTTONS];
bool mMouseButtonRelease[NUMBER_OF_MOUSE_BUTTONS];
double xScrollOffset = 0, yScrollOffset = 0;
double xScrollOffsetLast = 0, yScrollOffsetLast = 0;

double mX = 0;
double mY = 0;

double mDeltaX = 0;
double mDeltaY = 0;

void Input::clearInputs()
{
	memset(mPressedKeys, false, NUMBER_OF_KEYS);
	memset(mPressedKeysRepeated, false, NUMBER_OF_KEYS);
	memset(mReleasedKeys, false, NUMBER_OF_KEYS);
	memset(mMouseButtonPressedRepeat, false, NUMBER_OF_MOUSE_BUTTONS);
	memset(mMouseButtonPress, false, NUMBER_OF_MOUSE_BUTTONS);
	memset(mMouseButtonRelease, false, NUMBER_OF_MOUSE_BUTTONS);
	mDeltaX = 0;
	mDeltaY = 0;
}

void Input::setPressedKeyRepeated(uint key)
{
	mPressedKeysRepeated[key] = true;
}

void Input::setPressedKey(uint key)
{
	mPressedKeys[key] = true;
}

void Input::setReleasedKey(uint key)
{
	mReleasedKeys[key] = true;
}

bool Input::isPressedKeyRepeated(uint key)
{
	return mPressedKeysRepeated[key];
}

bool Input::wasPressedKey(uint key)
{
	return mPressedKeys[key];
}

bool Input::wasReleasedKey(uint key)
{
	return mReleasedKeys[key];
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
	return mMouseButtonRelease[mouseButton];
}

void Input::getScrollOffsetDelta(double & x, double & y)
{
	x = xScrollOffset;
	y = yScrollOffset;

	xScrollOffset = 0;
	yScrollOffset = 0;
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

void Input::setScrollOffset(double x, double y)
{
	xScrollOffset = x;
	yScrollOffset = y;
}
