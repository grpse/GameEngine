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

static inline bool replace(std::string& str, const std::string& from, const std::string& to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

// TODO: before render, after binding a shader, try set uniforms 

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

	struct Identifier
	{
		std::string VertexProgramID;
		std::string FragmentProgramID;
	};

	class Parser {

	public:
		Parser() {
			mVertexShaderSource = extractVertexShaderSourceFrom(mBaseGLSLShader);
			mFragmentShaderSource = extractFragmentShaderSourceFrom(mBaseGLSLShader);
			mVertexVariables = "";
			mFragmentVariables = "";
		}

		Identifier addProgram(const std::string& shader)
		{

			std::string vertexProgramName;
			std::string fragmentProgramName;
			std::string vertexProgram = extractProgramFunction("vertex", shader, vertexProgramName);
			std::string fragmentProgram = extractProgramFunction("fragment", shader, fragmentProgramName);

			Identifier program;
			std::string generatedVertexProgram = generateProgram(vertexProgram, vertexProgramName, program.VertexProgramID);
			std::string generatedFragmentProgram = generateProgram(fragmentProgram, fragmentProgramName, program.FragmentProgramID);
			assert(generatedVertexProgram.length());
			assert(generatedFragmentProgram.length());
			mVertexPrograms.push_back(generatedVertexProgram);
			mFragmentPrograms.push_back(generatedFragmentProgram);

			std::string uniforms = extractStringFromBeginEndBlock(shader, "#begin uniforms", "#end uniforms");

			mVertexVariables += extractStringFromBeginEndBlock(shader, "#begin vertex_variables", "#end vertex_variables") + "\n";
			mFragmentVariables += extractStringFromBeginEndBlock(shader, "#begin fragment_variables", "#end fragment_variables") + "\n";

			mUniforms.push_back(uniforms);

			return program;
		}

		std::string getVertexShaderSource() const
		{
			std::string generatedVertexShaderSource = buildProgramShader(mVertexPrograms, mVertexShaderSource);
			replace(generatedVertexShaderSource, "#VERTEX_VARIABLES", mVertexVariables);
			return generatedVertexShaderSource;
		}

		std::string getFragmentShaderSource() const
		{
			std::string generatedFragmentShaderSource = buildProgramShader(mFragmentPrograms, mFragmentShaderSource);
			replace(generatedFragmentShaderSource, "#FRAGMENT_VARIABLES", mFragmentVariables);
			return generatedFragmentShaderSource;
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
		uint mShaderProgramID = 0;

		std::string mVertexVariables;
		std::string mFragmentVariables;

		std::vector<std::string> mVertexPrograms;
		std::vector<std::string> mFragmentPrograms;

		std::string mBaseGLSLShader = R"(
		
#begin vertexshader 

#define MAX_LIGHTS_COUNT 10

// Vertex
layout(location = 0) in vec3 POSITION;
layout(location = 1) in vec3 NORMAL;
layout(location = 2) in vec3 TANGENT;
layout(location = 3) in vec3 BITANGENT;
layout(location = 4) in vec3 TEXCOORD0;
layout(location = 5) in vec3 TEXCOORD1;
layout(location = 6) in vec3 TEXCOORD2;
layout(location = 7) in vec3 TEXCOORD3;

// Basic uniforms
uniform mat4 WORLD;
uniform mat4 WORLDVIEW;
uniform mat4 WORLDVIEWPROJECTION;
uniform mat4 WORLDVIEWTRANSPOSE;
uniform mat4 WORLDVIEWINVERSE;
uniform mat4 WORLDVIEWINVERSETRANSPOSE;
uniform mat4 WORLDTRANSPOSE;
uniform mat4 WORLDINVERSE;
uniform mat4 WORLDINVERSETRANSPOSE;
uniform mat4 VIEW;
uniform mat4 VIEWTRANSPOSE;
uniform mat4 VIEWINVERSE;
uniform mat4 VIEWINVERSETRANSPOSE;
uniform mat4 PROJECTION;

uniform vec4 AmbientColor = vec4(0.3, 0.3, 0.3, 0.3);
uniform vec4 SpecularColor = vec4(1, 1, 1, 1);
uniform vec4 DiffuseColor = vec4(1, 1, 1, 1);

out vec3 PASS_POSITION;
out vec3 PASS_NORMAL;

struct Light {
	vec3 position;
	vec3 direction;
	vec4 color;
	float intensity;
	int type;
};

uniform Light light;

#VERTEX_VARIABLES

#UNIFORMS

subroutine vec4 programSubroutine();

subroutine uniform programSubroutine vertexProgramSubroutineInstance;

#PROGRAMS

void main()
{
	gl_Position = vertexProgramSubroutineInstance();
}

#end vertexshader



#begin fragmentshader

#define MAX_LIGHTS_COUNT 10

// Basic uniforms
uniform mat4 WORLD;
uniform mat4 WORLDVIEW;
uniform mat4 WORLDVIEWPROJECTION;
uniform mat4 WORLDVIEWTRANSPOSE;
uniform mat4 WORLDVIEWINVERSE;
uniform mat4 WORLDVIEWINVERSETRANSPOSE;
uniform mat4 WORLDTRANSPOSE;
uniform mat4 WORLDINVERSE;
uniform mat4 WORLDINVERSETRANSPOSE;
uniform mat4 VIEW;
uniform mat4 VIEWTRANSPOSE;
uniform mat4 VIEWINVERSE;
uniform mat4 VIEWINVERSETRANSPOSE;
uniform mat4 PROJECTION;

uniform vec4 AmbientColor = vec4(0.3, 0.3, 0.3, 0.3);
uniform vec4 SpecularColor = vec4(1, 1, 1, 1);
uniform vec4 DiffuseColor = vec4(1, 1, 1, 1);

in vec3 PASS_POSITION;
in vec3 PASS_NORMAL;

struct Light {
	vec3 position;
	vec3 direction;
	vec4 color;
	float intensity;
	int type;
};

uniform Light light;

#FRAGMENT_VARIABLES

#UNIFORMS

subroutine vec4 programSubroutine();

subroutine uniform programSubroutine fragmentProgramSubroutineInstance;

#PROGRAMS

void main()
{
	gl_FragColor = fragmentProgramSubroutineInstance();
}
#end fragmentshader

)";

		Shader::Queue mQueue;

		std::string buildProgramShader(const std::vector<std::string>& programs, const std::string& baseProgramSource) const
		{
			std::string programsSource = "";

			for (auto program : programs)
			{
				programsSource += program + "\n\n";
			}

			std::string shaderSourceCopy(baseProgramSource.begin(), baseProgramSource.end());


			std::string uniformsSource = "";

			for (auto uniformDef : mUniforms)
			{
				uniformsSource += uniformDef + "\n";
			}

			// TODO: replace all types from GENERIC SHADER TYPES TO SPECIFIC PLATFORM (GLSL, HLSL)
			transformInfoApiLanguageSpecificTypeUniforms(uniformsSource);
			transformInfoApiLanguageSpecificTypeProgram(programsSource);

			replace(shaderSourceCopy, "#UNIFORMS", uniformsSource + "\n");
			replace(shaderSourceCopy, "#PROGRAMS", programsSource + "\n");

			return shaderSourceCopy;
		}

		void transformInfoApiLanguageSpecificTypeUniforms(std::string& uniforms) const
		{
			if (Shader::getApiName() == ApiName::OpenGL)
			{
				while (replace(uniforms, "Matrix4", "uniform mat4"));
				while (replace(uniforms, "Color32", "uniform vec4"));
				while (replace(uniforms, "Vector4", "uniform vec4"));
				while (replace(uniforms, "Vector3", "uniform vec3"));
				while (replace(uniforms, "Vector2", "uniform vec2"));
				while (replace(uniforms, "Texture2D", "uniform sampler2D"));
				while (replace(uniforms, "CubeMap", "uniform samplerCube"));
			}
		}

		void transformInfoApiLanguageSpecificTypeProgram(std::string& program) const
		{
			if (Shader::getApiName() == ApiName::OpenGL)
			{
				while (replace(program, "Matrix4", "mat4"));
				while (replace(program, "Color32", "vec4"));
				while (replace(program, "Vector4", "vec4"));
				while (replace(program, "Vector3", "vec3"));
				while (replace(program, "Vector2", "vec2"));
				while (replace(program, "Texture2D", "sampler2D"));
				while (replace(program, "CubeMap", "samplerCube"));
			}
		}

		std::string extractProgramFunction(const std::string& program, const std::string& shader, std::string& programName)
		{
			std::string programMacro = "#" + program;
			size_t lastPositionOfMacroDefinition;
			programName = extractMacroValue(programMacro, shader, lastPositionOfMacroDefinition);

			std::string afterMacroDefinition = shader.substr(lastPositionOfMacroDefinition + programMacro.length());

			size_t startPositionOfProgramFunction = afterMacroDefinition.find(programName);
			if (startPositionOfProgramFunction == std::string::npos) return "";


			startPositionOfProgramFunction += programName.length();

			size_t endPositionOfVertexShaderFunction = 0;

			// find first \n that comes before the vertex function definition
			for (size_t i = startPositionOfProgramFunction; i >= 0; i--)
			{
				if (afterMacroDefinition[i] == '\n')
				{
					startPositionOfProgramFunction = i + 1;
					break;
				}
				else
				{
					startPositionOfProgramFunction = i;
				}
			}

			// find {} matchs to substring the full vertex shader function
			size_t curlyBracesCount = 0;
			for (size_t i = startPositionOfProgramFunction; i < afterMacroDefinition.length(); i++)
			{
				if (afterMacroDefinition[i] == '{')
				{
					curlyBracesCount++;

				}
				else if (afterMacroDefinition[i] == '}')
				{
					curlyBracesCount--;

					if (curlyBracesCount == 0)
					{
						endPositionOfVertexShaderFunction = i + 1;
						break;
					}
				}
			}

			if (curlyBracesCount != 0)
			{
				return "";
			}

			return afterMacroDefinition.substr(startPositionOfProgramFunction, endPositionOfVertexShaderFunction - startPositionOfProgramFunction);
		}

		std::string generateProgram(std::string program, std::string programName, std::string& outProgramID)
		{
			outProgramID = "__program_" + std::to_string(mShaderProgramID++);

			std::string programCopy(program.begin(), program.end());

			replace(programCopy, programName, outProgramID);

			programCopy = "subroutine (programSubroutine)\n" + programCopy;

			return programCopy;
		}

		Shader::Queue extractShaderQueue(const std::string& shader)
		{
			size_t lastPosition;
			std::string queueDescription = extractMacroValue("#queue", shader, lastPosition);
			if (queueDescription == "Opaque")
				return Shader::Queue::Opaque;
			else if (queueDescription == "Cutoff")
				return Shader::Queue::Cutoff;
			else if (queueDescription == "Transparent")
				return Shader::Queue::Transparent;
			else
				return Shader::Queue::Opaque;
		}

		std::string extractMacroValue(const std::string& MACRO, const std::string& shader, size_t& lastPosition)
		{
			size_t beginPosition = shader.find(MACRO);
			if (beginPosition == std::string::npos) return "";


			const std::string& partialCodeFromShader = shader.substr(beginPosition + MACRO.size());
			size_t endPosition = partialCodeFromShader.find("\n");
			lastPosition = beginPosition + endPosition;
			std::string macroValue = partialCodeFromShader.substr(0, endPosition);

			trim(macroValue);

			return macroValue;
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