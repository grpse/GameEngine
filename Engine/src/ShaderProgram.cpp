#include <GL/glew.h>
#include <cstring>
#include <vector>
#include <iostream>
#include "GLErrorHandling.h"
#include "ShaderProgram.h"
#include "ShaderParser.h"

#define STRINGIFY(value) (#value)

ShaderProgram::ShaderProgram() 
{
	VERSION = STRINGIFY(#version 430\n);
	PRECODE_VERTEX = "";
	PRECODE_FRAGMENT = "";

	memset(&mUniformsUse, -1, sizeof(mUniformsUse));
	memset(&mAttributesUse, -1, sizeof(mAttributesUse));
}

ShaderProgram::~ShaderProgram() 
{
	GLCall(glDeleteShader(mShaderProgram));
}

void ShaderProgram::setUniform(uint uniform, uint i)
{
	GLCall(glUniform1i(uniform, i));
}

void ShaderProgram::setUniform(uint uniform, float v)
{
	GLCall(glUniform1f(uniform, v));
}

void ShaderProgram::setUniform(uint uniform, const Color32 & v)
{
	GLCall(glUniform4fv(uniform, 1, Math::value_ptr(v)));
}

void ShaderProgram::setUniform(uint uniform, const Vector3& v)
{
	GLCall(glUniform3fv(uniform, 1, Math::value_ptr(v)));
}

void ShaderProgram::setUniform(uint uniform, const Vector2& v)
{
	GLCall(glUniform2fv(uniform, 1, Math::value_ptr(v)));
}

void ShaderProgram::setUniform(uint uniform, const Matrix4& m)
{
	GLCall(glUniformMatrix4fv(uniform, 1, GL_FALSE, Math::value_ptr(m)));
}

void ShaderProgram::setUniform(const char * uniform, uint i)
{
	uint uniformLocation = getUniformLocation(uniform);
	setUniform(uniformLocation, i);
}

void ShaderProgram::setUniform(const char * uniform, const Color32 & v)
{
	uint uniformLocation = getUniformLocation(uniform);
	setUniform(uniformLocation, v);
}

void ShaderProgram::setUniform(const char * uniform, const Vector3& v)
{
	uint uniformLocation = getUniformLocation(uniform);
	setUniform(uniformLocation, v);
}

void ShaderProgram::setUniform(const char * uniform, const Vector2& v)
{
	uint uniformLocation = getUniformLocation(uniform);
	setUniform(uniformLocation, v);
}

void ShaderProgram::setUniform(const char * uniform, const Matrix4& m)
{
	uint uniformLocation = getUniformLocation(uniform);
	setUniform(uniformLocation, m);
}

void ShaderProgram::setProjectionMatrix(const Matrix4& projection)
{
	if (mUniformsUse.Projection < 0)
		mUniformsUse.Projection = getUniformLocation(PROJECTION);

	setUniform(mUniformsUse.Projection, projection);
}

void ShaderProgram::setViewMatrix(const Matrix4& view)
{
	if (mUniformsUse.View < 0)
		mUniformsUse.View = getUniformLocation(VIEW);

	setUniform(mUniformsUse.View, view);
}

void ShaderProgram::setWorldMatrix(const Matrix4 & world)
{
	if (mUniformsUse.World < 0)
		mUniformsUse.World = getUniformLocation(WORLD);

	setUniform(mUniformsUse.World, world);
}

void ShaderProgram::setWorldViewMatrix(const Matrix4 & worldView)
{
	if (mUniformsUse.WorldView < 0)
		mUniformsUse.WorldView = getUniformLocation(WORLDVIEW);

	setUniform(mUniformsUse.WorldView, worldView);
}

void ShaderProgram::setWorldViewProjectionMatrix(const Matrix4& worldViewProjection)
{
	if (mUniformsUse.WorldViewProjection < 0)
		mUniformsUse.WorldViewProjection = getUniformLocation(WORLDVIEWPROJECTION);

	setUniform(mUniformsUse.WorldViewProjection, worldViewProjection);
}

uint ShaderProgram::getUniformLocation(const char* uniform)
{
	GLCall(uint uniformLocation = glGetUniformLocation(mShaderProgram, uniform));
	//std::cout << "Uniform location: " << uniform << " " << uniformLocation << std::endl;
	return uniformLocation;
}

uint ShaderProgram::getAttributeLocation(const char* uniform)
{
	GLCall(uint attributeLocation = glGetAttribLocation(mShaderProgram, uniform));
	return attributeLocation;
}

void ShaderProgram::setCustomUniform(std::string customUniform)
{
	PRECODE_VERTEX += customUniform + STRINGIFY(\n);
}

void ShaderProgram::buildShadersFromSource(std::string shaderSource) 
{
	//TODO: Generalize BEGIN/END codes to remove get from shader source resource the distinction
	//		on what shader is going to be compiled and linked.
	//TODO: Link method should also be able to receive multiples shaders to link to shader program
	GLCall(mShaderProgram = glCreateProgram());

	Shader::Parser parser;

	parser.parse(shaderSource.c_str());
		
	std::cout << "VERTEX SHADER" << std::endl;
	std::string& vertShaderSource = VERSION + parser.getVertexShaderSource();
	std::cout << vertShaderSource << std::endl;
	const char* vertShaderSourceStr = vertShaderSource.c_str();
	buildVertShaderFromSource(vertShaderSourceStr);

	std::cout << "FRAGMENT SHADER" << std::endl;
	std::string& fragShaderSource = VERSION + parser.getFragmentShaderSource();
	std::cout << fragShaderSource << std::endl;
	const char* fragShaderSourceStr = fragShaderSource.c_str();
	buildFragShaderFromSource(fragShaderSourceStr);

	link();
}

void ShaderProgram::start()
{
	GLCall(glUseProgram(mShaderProgram));
}

void ShaderProgram::stop()
{
	GLCall(glUseProgram(0));
}

void ShaderProgram::buildVertShaderFromSource(const char * vertShaderSource)
{
	mVertShader = compileShaderFromSource(GL_VERTEX_SHADER, vertShaderSource);
}

void ShaderProgram::buildFragShaderFromSource(const char * fragShaderSource)
{
	mFragShader = compileShaderFromSource(GL_FRAGMENT_SHADER, fragShaderSource);
}

uint ShaderProgram::compileShaderFromSource(uint shaderType, const char* source) 
{
	GLCall(uint shader = glCreateShader(shaderType));
	GLCall(glShaderSource(shader, 1, &source, 0));	
	GLCall(glCompileShader(shader));

	GLint isCompiled = 0;
	GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled));
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		GLCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength));

		//The maxLength includes the NULL character
		auto infoLog = new GLchar[maxLength + 1];
		GLCall(glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog));
		std::cout << STRINGIFY(OpenGL Compilation Error: ) << infoLog << std::endl;
		GLCall(glDeleteShader(shader));
		return -1;
	}

	return shader;
}

void ShaderProgram::link()
{
	//Attach our shaders to our program
	GLCall(glAttachShader(mShaderProgram, mVertShader));
	GLCall(glAttachShader(mShaderProgram, mFragShader));

	//Link our program
	GLCall(glLinkProgram(mShaderProgram));

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	GLCall(glGetProgramiv(mShaderProgram, GL_LINK_STATUS, (int *)&isLinked));
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		GLCall(glGetProgramiv(mShaderProgram, GL_INFO_LOG_LENGTH, &maxLength));

		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		GLCall(glGetProgramInfoLog(mShaderProgram, maxLength, &maxLength, &infoLog[0]));

		//We don't need the program anymore.
		GLCall(glDeleteProgram(mShaderProgram));
		//Don't leak shaders either.
		GLCall(glDeleteShader(mVertShader));
		GLCall(glDeleteShader(mFragShader));

		const char* erroString = (const char*)&infoLog[0];
		//Use the infoLog as you see fit.
		std::cout << STRINGIFY(OpenGL: Linker Error: ) << erroString << std::endl;

		//In this simple program, we'll just leave
		return;
	}

	//Always detach shaders after a successful link.
	GLCall(glDetachShader(mShaderProgram, mVertShader));
	GLCall(glDetachShader(mShaderProgram, mFragShader));
}
