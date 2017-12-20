#pragma once

#include <GL/glew.h>
#include <cassert>
#include <iostream>

static void GLClearError();
static bool GLLogCall(const char *function, const char *filename, int line);
static const char *GLErrorCodeToString(GLenum error);

#define GLCall(x) GLClearError(); x; assert(GLLogCall(#x, __FILE__, __LINE__));

inline static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

inline static bool GLLogCall(const char *function, const char *filename, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout	<< "[OpenGL Error] (" << error << " = "
					<< GLErrorCodeToString(error) << ") "
					<< function << " " << filename << ":"
					<< line << std::endl;
		return false;
	}

	return true;
}

inline static const char *GLErrorCodeToString(GLenum error)
{

	switch (error)
	{
	case GL_INVALID_ENUM:
		return "GL_INVALID_ENUM";
		break;

	case GL_INVALID_VALUE:
		return "GL_INVALID_VALUE";
		break;

	case GL_INVALID_OPERATION:
		return "GL_INVALID_OPERATION";
		break;

	case GL_STACK_OVERFLOW:
		return "GL_STACK_OVERFLOW";
		break;

	case GL_STACK_UNDERFLOW:
		return "GL_STACK_UNDERFLOW";
		break;

	case GL_OUT_OF_MEMORY:
		return "GL_OUT_OF_MEMORY";
		break;

	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return "GL_INVALID_FRAMEBUFFER_OPERATION";
		break;

	case GL_CONTEXT_LOST:
		return "GL_CONTEXT_LOST";
		break;

	case GL_TABLE_TOO_LARGE:
		return "GL_TABLE_TOO_LARGE1";
		break;

	default:
		return "UNKNOWN ERROR";
		break;
	}
}