#include "Input.h"

bool mPressedKeys[NUMBER_OF_KEYS];

void Input::clearInputs()
{
	memset(mPressedKeys, false, NUMBER_OF_KEYS);
}

void Input::setPressedKey(uint key)
{
	mPressedKeys[key] = true;
}

bool Input::isPressedKey(uint key)
{
	return mPressedKeys[key];
}