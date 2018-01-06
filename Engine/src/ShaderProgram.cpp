#include <GL/glew.h>
#include <cstring>
#include <vector>
#include <iostream>
#include "GLErrorHandling.h"
#include "ShaderProgram.h"

#define STRINGIFY(value) (#value)

ShaderProgram::ShaderProgram() 
{
	
	PRECODE_VERTEX = STRINGIFY(#version 130\n);
	PRECODE_FRAGMENT = STRINGIFY(#version 130\n);

	ATTRIBUTE_VERTEX_POSITION = "";
	ATTRIBUTE_NORMAL_POSITION = "";
	ATTRIBUTE_TEXTURECOORD0 = "";
	ATTRIBUTE_TEXTURECOORD1 = "";
	ATTRIBUTE_TEXTURECOORD2 = "";

	//mUniformsUse = { -1, -1, -1, -1, -1 };
	memset(&mUniformsUse, -1, sizeof(mUniformsUse));
	//mAttributesUse = { -1, -1, -1, -1, -1 };
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
		mUniformsUse.Projection = getUniformLocation(STRINGIFY(Projection));

	setUniform(mUniformsUse.Projection, projection);
}

void ShaderProgram::setViewMatrix(const Matrix4& view)
{
	if (mUniformsUse.View < 0)
		mUniformsUse.View = getUniformLocation(STRINGIFY(View));

	setUniform(mUniformsUse.View, view);
}

void ShaderProgram::setWorldMatrix(const Matrix4 & world)
{
	if (mUniformsUse.World < 0)
		mUniformsUse.World = getUniformLocation(STRINGIFY(World));

	setUniform(mUniformsUse.World, world);
}

void ShaderProgram::setWorldViewMatrix(const Matrix4 & worldView)
{
	if (mUniformsUse.WorldView < 0)
		mUniformsUse.WorldView = getUniformLocation(STRINGIFY(WorldView));

	setUniform(mUniformsUse.WorldView, worldView);
}

void ShaderProgram::setWorldViewProjectionMatrix(const Matrix4& worldViewProjection)
{
	if (mUniformsUse.WorldViewProjection < 0)
		mUniformsUse.WorldViewProjection = getUniformLocation(STRINGIFY(WorldViewProjection));

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

void ShaderProgram::useWorldMatrix() 
{
	PRECODE_VERTEX += STRINGIFY(uniform mat4 World;\n);
}

void ShaderProgram::useViewMatrix() 
{
	PRECODE_VERTEX += STRINGIFY(uniform mat4 View;\n);
}

void ShaderProgram::useWorldViewMatrix() 
{
	PRECODE_VERTEX += STRINGIFY(uniform mat4 WorldView;\n);
}

void ShaderProgram::useProjectionMatrix() 
{
	PRECODE_VERTEX += STRINGIFY(uniform mat4 Projection;\n);
}

void ShaderProgram::useWorldViewProjectionMatrix()
{
	PRECODE_VERTEX += STRINGIFY(uniform mat4 WorldViewProjection; \n);
}

void ShaderProgram::useVertexAttribute() 
{
	ATTRIBUTE_VERTEX_POSITION += STRINGIFY(in vec3 VertexPosition_ModelSpace;\n);
}

void ShaderProgram::useNormalAttribute() 
{
	ATTRIBUTE_NORMAL_POSITION += STRINGIFY(in vec3 VertexNormal_ModelSpace;\n);
}

void ShaderProgram::useTextureCoord0Attribute()
{
	ATTRIBUTE_TEXTURECOORD0 += STRINGIFY(in vec2 TextureCoord0;\n);
}

void ShaderProgram::useTextureCoord1Attribute()
{
	ATTRIBUTE_TEXTURECOORD1 += STRINGIFY(in vec2 TextureCoord1;\n);
}

void ShaderProgram::useTextureCoord2Attribute()
{
	ATTRIBUTE_TEXTURECOORD2 += STRINGIFY(in vec2 TextureCoord2;\n);
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
	
	std::string source;
	source.assign(shaderSource.c_str(), shaderSource.length());

	std::string BEGIN_VERTEXSHADER = STRINGIFY(#BEGIN VERTEXSHADER);
	std::string END_VERTEXSHADER = STRINGIFY(#END VERTEXSHADER);

	uint beginVertShaderPosition = source.find(BEGIN_VERTEXSHADER) + BEGIN_VERTEXSHADER.length();
	uint endVertShaderPosition = source.find(END_VERTEXSHADER);
	uint vertSourcePositionsCount = endVertShaderPosition - beginVertShaderPosition;
	std::string vertShaderSource = 
		PRECODE_VERTEX + 
		ATTRIBUTE_VERTEX_POSITION +
		ATTRIBUTE_NORMAL_POSITION +
		ATTRIBUTE_TEXTURECOORD0 +
		ATTRIBUTE_TEXTURECOORD1 +
		ATTRIBUTE_TEXTURECOORD2 +
		STRINGIFY(\n) + source.substr(beginVertShaderPosition, vertSourcePositionsCount);
		
	std::cout << vertShaderSource << std::endl;
	const char* vertShaderSourceStr = vertShaderSource.c_str();
	buildVertShaderFromSource(vertShaderSourceStr);

	std::cout << std::endl;

	std::string BEGIN_FRAGMENTSHADER = STRINGIFY(#BEGIN FRAGMENTSHADER);
	std::string END_FRAGMENTSHADER = STRINGIFY(#END FRAGMENTSHADER);

	uint beginFragShaderPosition = source.find(BEGIN_FRAGMENTSHADER) + BEGIN_FRAGMENTSHADER.length();
	uint endFragShaderPosition = source.find(END_FRAGMENTSHADER);
	uint fragSourcePositionsCount = endFragShaderPosition - beginFragShaderPosition;
	std::string fragShaderSource = PRECODE_FRAGMENT + STRINGIFY(\n) + source.substr(beginFragShaderPosition, fragSourcePositionsCount);
	
	// std::cout << fragShaderSource << std::endl;
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
