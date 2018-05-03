#include <GL/glew.h>
#include <cstring>
#include <vector>
#include <iostream>
#include "GLErrorHandling.h"
#include "ShaderProgram.h"
#include "ShaderParser.h"

ShaderProgram* mCurrentBoundShaderProgram = nullptr;
Shader::Parser parser;
int ShaderProgram::mShaderProgram = -1;
int ShaderProgram::mVertShader = -1;
int ShaderProgram::mFragShader = -1;
std::string ShaderProgram::VERSION = STRINGIFY(#version 400\n);
ShaderProgram::UniformsUse ShaderProgram::mUniformsUse = { -1, -1, -1, -1, -1, -1, -1 };
ShaderProgram::AttributesUse ShaderProgram::mAttributesUse = { -1, -1, -1, -1, -1, -1, -1 };
std::map<std::string, ShaderProgram::ProgramIndexLocation > ShaderProgram::mVertProgramName_ID;
std::map<std::string, ShaderProgram::ProgramIndexLocation > ShaderProgram::mFragProgramName_ID;


ShaderProgram::ShaderProgram() 
{
	memset(&mUniformsUse, -1, sizeof(mUniformsUse));
	memset(&mAttributesUse, -1, sizeof(mAttributesUse));
}

ShaderProgram::ShaderProgram(const ShaderProgram & other)
{
	mFragmentShaderID = other.mFragmentShaderID;
	mVertexShaderID = other.mVertexShaderID;
}

ShaderProgram::~ShaderProgram() 
{
	//TODO: Should delete program, but only when no one will use it
	/*

	if (mShaderProgram >= 0)
	{
		GLCall(glDeleteProgram(mShaderProgram));
		mShaderProgram = -1;
	}
	*/
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

void ShaderProgram::setUniform(uint uniform, const Texture2D& t)
{
	// TODO: This can only bind one texture to current shader program
	GLCall(glUniform1i(uniform, t.getIndex()));
}

void ShaderProgram::setUniform(uint uniform, const CubeMap& c)
{
	c.bind();
	GLCall(glUniform1i(uniform, c.getIndex()));
}

void ShaderProgram::setInteger(uint uniform, int i)
{
	GLCall(glUniform1i(uniform, i));
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

void ShaderProgram::setWorldInverseTranspose(const Matrix4& worldInverseTranspose)
{
	if (mUniformsUse.WorldInverseTranspose < 0)
		mUniformsUse.WorldInverseTranspose = getUniformLocation(WORLDINVERSETRANSPOSE);

	setUniform(mUniformsUse.WorldInverseTranspose, worldInverseTranspose);
}

void ShaderProgram::setWorldViewInverseTranspose(const Matrix4& worldViewInverseTranspose)
{
	if (mUniformsUse.WorldViewInverseTranspose < 0)
		mUniformsUse.WorldViewInverseTranspose = getUniformLocation(WORLDVIEWINVERSETRANSPOSE);

	setUniform(mUniformsUse.WorldViewInverseTranspose, worldViewInverseTranspose);
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

int ShaderProgram::getUniformLocation(const char* uniform)
{
	GLCall(uint uniformLocation = glGetUniformLocation(mShaderProgram, uniform));
	//std::cout << "Uniform location: " << uniform << " " << uniformLocation << std::endl;
	return uniformLocation;
}

int ShaderProgram::getAttributeLocation(const char* uniform)
{
	GLCall(uint attributeLocation = glGetAttribLocation(mShaderProgram, uniform));
	return attributeLocation;
}

ShaderProgram& ShaderProgram::getCurrentBound()
{
	return *mCurrentBoundShaderProgram;
}

void ShaderProgram::build()
{
	//TODO: Generalize BEGIN/END codes to remove get from shader source resource the distinction
	//		on what shader is going to be compiled and linked.
	//TODO: Link method should also be able to receive multiples shaders to link to shader program
	GLCall(mShaderProgram = glCreateProgram());
	
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

	GLCall(glUseProgram(mShaderProgram));

	for (auto& keyPair : mVertProgramName_ID)
	{
		const char* vertProgramNamePtr = keyPair.first.c_str();
		GLCall(keyPair.second.index = glGetSubroutineIndex(mShaderProgram, GL_VERTEX_SHADER, vertProgramNamePtr));
	}

	for (auto& keyPair : mFragProgramName_ID)
	{
		const char* fragProgramNamePtr = keyPair.first.c_str();
		GLCall(keyPair.second.index = glGetSubroutineIndex(mShaderProgram, GL_FRAGMENT_SHADER, fragProgramNamePtr));
	}
}

void ShaderProgram::addProgram(std::string shaderSource)
{
	Shader::Identifier identifier = parser.addProgram(shaderSource);
	mFragmentShaderID = identifier.FragmentProgramID;
	mVertexShaderID = identifier.VertexProgramID;

	mFragProgramName_ID[mFragmentShaderID] = { 1, -1 };
	mVertProgramName_ID[mVertexShaderID] = { 1, -1 };
}

void ShaderProgram::bind() const
{
	GLCall(glUseProgram(mShaderProgram));
	mCurrentBoundShaderProgram = (ShaderProgram*)this;
	GLCall(glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &mVertProgramName_ID[mVertexShaderID].index));
	GLCall(glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &mFragProgramName_ID[mFragmentShaderID].index));
}

void ShaderProgram::unbind() const
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

int ShaderProgram::compileShaderFromSource(uint shaderType, const char* source) 
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
