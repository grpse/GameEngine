#include "GLErrorHandling.h"
#include <iostream>

bool FirstCall = true;

void GLClearError()
{
	/*
	while (glGetError() != GL_NO_ERROR)
	{
	std::cout << "GL ERROR" << std::endl;
	}
	*/

	while (GLenum error = glGetError())
	{
		if (FirstCall) {
			FirstCall = false;
			break;
		}
		std::cout << "[OpenGL Error] (" << error << " = "
			<< GLErrorCodeToString(error) << ") " << std::endl;
	}
}

bool GLLogCall(const char *function, const char *filename, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << " = "
			<< GLErrorCodeToString(error) << ") "
			<< function << " " << filename << ":"
			<< line << std::endl;
		return false;
	}

	return true;
}


const char *GLErrorCodeToString(GLenum error)
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