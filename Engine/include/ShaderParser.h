#pragma once
#pragma once

#include <algorithm> 
#include <cctype>
//#include <locale>
#include <vector>
#include <string>
#include <iostream>
#include "Texture2D.h"
#include "LinearMath.h"
#include "MatricesNamesDefines.h"
#include "AttributesNamesDefines.h"


typedef unsigned int uint;

// trim from start (in place)
static inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
	ltrim(s);
	rtrim(s);
}

// TODO: before render, after binding a shader, try set uniforms 

#define STRINGIFY(x) (#x)

namespace Shader {

	enum class ApiName {
		OpenGL,
		DirectX
	};

	enum class Queue {
		Opaque,
		Cutoff,
		Transparent,

		Count // Count number of enums has here
	};



	ApiName getApiName()
	{
		return ApiName::OpenGL;
	}

	struct Attribute {
		std::string name;
		uint location;
	};

	struct Uniform {
		std::string name;
		uint location;
	};

	class GlobalUniforms {

	private:


	};

	/*
	if renderable.hasPreProcessing? then
	... put on appropriated vector space



	for each renderable in RenderablesThatHasPressProcesses then


	for each renderable in SceneRenderables then




	for each Camera in SceneCameras then

	for each Material in renderable.getMaterials() then

	Material.setCameraAvailableUniforms Camera

	if Material.hasLightsArrayUniform? then
	Material.setLightsAvailableUniform Lights

	renderable.render Camera, Lights, Actor, Renderer

	*/

	class Parser {

	public:
		void parse(const char* source) {

			std::string shader = source;

			// SETUP ALL DEFAULT MATRICES USED IN SHADER CODE
			usingUniformOnShaderSourceSetUniform<Matrix4>(WORLD, shader);
			usingUniformOnShaderSourceSetUniform<Matrix4>(WORLDVIEW, shader);
			usingUniformOnShaderSourceSetUniform<Matrix4>(WORLDVIEWPROJECTION, shader);
			usingUniformOnShaderSourceSetUniform<Matrix4>(WORLDVIEWTRANSPOSE, shader);
			usingUniformOnShaderSourceSetUniform<Matrix4>(WORLDVIEWINVERSE, shader);
			usingUniformOnShaderSourceSetUniform<Matrix4>(WORLDVIEWINVERSETRANSPOSE, shader);
			usingUniformOnShaderSourceSetUniform<Matrix4>(WORLDTRANSPOSE, shader);
			usingUniformOnShaderSourceSetUniform<Matrix4>(WORLDINVERSE, shader);
			usingUniformOnShaderSourceSetUniform<Matrix4>(WORLDINVERSETRANSPOSE, shader);
			usingUniformOnShaderSourceSetUniform<Matrix4>(VIEW, shader);
			usingUniformOnShaderSourceSetUniform<Matrix4>(VIEWTRANSPOSE, shader);
			usingUniformOnShaderSourceSetUniform<Matrix4>(VIEWINVERSE, shader);
			usingUniformOnShaderSourceSetUniform<Matrix4>(VIEWINVERSETRANSPOSE, shader);
			usingUniformOnShaderSourceSetUniform<Matrix4>(PROJECTION, shader);

			// RESPECT THIS FIXED ORDER ON SHADER
			usingAttributeOnShaderSourceSetUniform<Vector3>(POSITION, shader);
			usingAttributeOnShaderSourceSetUniform<Vector3>(NORMAL, shader);
			usingAttributeOnShaderSourceSetUniform<Vector3>(TANGENT, shader);
			usingAttributeOnShaderSourceSetUniform<Vector3>(BITANGENT, shader);
			usingAttributeOnShaderSourceSetUniform<Vector2>(TEXCOORD0, shader);
			usingAttributeOnShaderSourceSetUniform<Vector2>(TEXCOORD1, shader);
			usingAttributeOnShaderSourceSetUniform<Vector2>(TEXCOORD2, shader);
			usingAttributeOnShaderSourceSetUniform<Vector2>(TEXCOORD3, shader);


			mVertexShaderSource = extractVertexShaderSourceFrom(shader);
			mFragmentShaderSource = extractFragmentShaderSourceFrom(shader);
			mQueue = extractShaderQueue(shader);
		}

		std::string getVertexShaderSource() const
		{
			return mAttributesBlock + mUniformsBlock + mVertexShaderSource;
		}

		std::string getFragmentShaderSource() const
		{
			return mUniformsBlock + mFragmentShaderSource;
		}

		const Shader::Queue getQueue() const
		{
			return mQueue;
		}

	private:
		std::string mVertexShaderSource;
		std::string mFragmentShaderSource;
		std::string mAttributesBlock;
		std::string mUniformsBlock;
		std::vector<std::string> mAttributes;
		std::vector<std::string> mUniforms;
		uint mAttributeLocation = 0;

		Shader::Queue mQueue;

		std::vector<std::string> extractUniforms(const std::string& shader)
		{
			const std::string UNIFORM = "uniform";

		}

		Shader::Queue extractShaderQueue(const std::string& shader)
		{
			const std::string BEGIN_QUEUE = "#queue";
			size_t beginPosition = shader.find(BEGIN_QUEUE);
			if (beginPosition == std::string::npos) return Shader::Queue::Opaque;


			const std::string& partialCodeFromShader = shader.substr(beginPosition + BEGIN_QUEUE.size());
			size_t endPosition = partialCodeFromShader.find("\n");

			std::string queueDescription = partialCodeFromShader.substr(0, endPosition);

			trim(queueDescription);

			if (queueDescription == "Opaque")
				return Shader::Queue::Opaque;
			else if (queueDescription == "Cutoff")
				return Shader::Queue::Cutoff;
			else if (queueDescription == "Transparent")
				return Shader::Queue::Transparent;
			else
				return Shader::Queue::Opaque;
		}

		std::string extractVertexShaderSourceFrom(const std::string& shader)
		{
			const std::string BEGIN_VERTEXSHADER = "#begin vertexshader";
			const std::string END_VERTEXSHADER = "#end vertexshader";

			return extractStringFromBeginEndBlock(shader, BEGIN_VERTEXSHADER, END_VERTEXSHADER);
		}

		std::string extractFragmentShaderSourceFrom(const std::string& shader)
		{
			const std::string BEGIN_FRAGMENTSHADER = "#begin fragmentshader";
			const std::string END_FRAGMENTSHADER = "#end fragmentshader";

			return extractStringFromBeginEndBlock(shader, BEGIN_FRAGMENTSHADER, END_FRAGMENTSHADER);
		}

		std::string extractStringFromBeginEndBlock(const std::string& str, const std::string& beginMark, const std::string& endMark)
		{
			size_t beginPosition = str.find(beginMark);
			size_t endPosition = str.find(endMark);

			if (beginPosition != std::string::npos && endPosition != std::string::npos)
			{
				size_t startPosition = beginPosition + beginMark.size();
				size_t length = endPosition - startPosition;
				return str.substr(startPosition, length);
			}

			return "";
		}

		template <typename AttributeType>
		bool usingAttributeOnShaderSourceSetUniform(const std::string& attributeName, const std::string& shaderSource)
		{
			if (stringMatchExpression(shaderSource, attributeName)) {
				setAttribute<AttributeType>(attributeName);
				return true;
			}
			return false;
		}

		template <typename UniformType>
		bool usingUniformOnShaderSourceSetUniform(const std::string& uniformName, const std::string& shaderSource)
		{
			if (stringMatchExpression(shaderSource, uniformName)) {
				setUniform<UniformType>(uniformName);
				return true;
			}
			return false;
		}

		template <typename AttributeType>
		void setAttribute(const std::string& attribute)
		{
			std::string attribTypeString = convertForApiType(typeid(AttributeType).hash_code());
			mAttributesBlock += "in " + attribTypeString + " " + attribute + ";\n";
		}

		template <typename UniformType>
		void setUniform(const std::string& uniform)
		{
			std::string uniformTypeString = convertForApiType(typeid(UniformType).hash_code());
			mUniformsBlock += "uniform " + uniformTypeString + " " + uniform + ";\n";
		}

		bool stringMatchExpression(const std::string& str, const std::string& expression)
		{
			return str.find(expression) != std::string::npos;
		}

		std::string convertForApiType(size_t typeHash)
		{
			if (Shader::getApiName() == ApiName::OpenGL)
			{
				if (typeid(Texture2D).hash_code() == typeHash)
					return "sampler2D";

				if (typeid(Matrix4).hash_code() == typeHash)
					return "mat4";

				if (typeid(Color32).hash_code() == typeHash)
					return "vec4";

				//if (typeid(Vector4).hash_code() == typeHash)
					//return "vec4";

				if (typeid(Vector3).hash_code() == typeHash)
					return "vec3";

				if (typeid(Vector2).hash_code() == typeHash)
					return "vec2";

				if (typeid(int).hash_code() == typeHash)
					return "int";

				if (typeid(float).hash_code() == typeHash)
					return "float";
			}

			return "";
		}
	};
};



static inline std::ostream& operator<<(std::ostream& out, Shader::Queue queueType)
{
	std::string queueName = "";

	switch (queueType)
	{
	case Shader::Queue::Opaque:
		queueName = "Opaque";
		break;

	case Shader::Queue::Transparent:
		queueName = "Transparent";
		break;

	case Shader::Queue::Cutoff:
		queueName = "Cutoff";
		break;

	default:
		queueName = "Queue Name Not Defined";
		break;
	}

	out << queueName;
	return out;
}