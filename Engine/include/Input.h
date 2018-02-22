#pragma once
#include <SDL.h>
#include "Typedefs.h"
#include <string.h>
#define NUMBER_OF_KEYS 256

const uint ArrowUp = 82;
const uint ArrowDown = 81;
const uint ArrowLeft = 80;
const uint ArrowRight = 79;
const uint Space = 44;
const uint W = 26;
const uint S = 22;
const uint D = 7;
const uint A = 4;

class Input {
public:
	static void clearInputs();
	static void setPressedKey(uint key);
	static bool isPressedKey(uint key);
};