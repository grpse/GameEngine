#pragma once

#include <GL/glew.h>
#include <cassert>
#include <iostream>

static bool GameLoopRunning;

void GLClearError();
bool GLLogCall(const char *function, const char *filename, int line);
const char *GLErrorCodeToString(GLenum error);

#define GLCall(x) GLClearError(); x; assert(GLLogCall(#x, __FILE__, __LINE__));