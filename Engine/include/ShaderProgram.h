#pragma once
#include "LinearMath.h"
#include "Texture2D.h"
#include "CubeMap.h"
#include "Typedefs.h"
#include <string>
#include <map>

class ShaderProgram {

public:
	ShaderProgram();
	~ShaderProgram();

	void setUniform(uint uniform, uint i);
	void setUniform(uint uniform, float v);
	void setUniform(uint uniform, const Color32& v);
	void setUniform(uint uniform, const Vector3& v);
	void setUniform(uint uniform, const Vector2& v);
	void setUniform(uint uniform, const Matrix4& m);
	void setUniform(uint uniform, const Texture2D& t);
	void setUniform(uint uniform, const CubeMap& c);
	void setInteger(uint uniform, int i);

	void setUniform(const char* uniform, uint i);
	void setUniform(const char* uniform, const Color32& v);
	void setUniform(const char* uniform, const Vector3& v);
	void setUniform(const char* uniform, const Vector2& v);
	void setUniform(const char* uniform, const Matrix4& m);

	void setWorldMatrix(const Matrix4& world);
	void setViewMatrix(const Matrix4& view);
	void setWorldViewInverseTranspose(const Matrix4& worldViewInverseTranspose);
	void setWorldInverseTranspose(const Matrix4& worldInverseTranspose);
	void setWorldViewMatrix(const Matrix4& worldView);
	void setProjectionMatrix(const Matrix4& projection);
	void setWorldViewProjectionMatrix(const Matrix4& worldViewProjection);

	int getUniformLocation(const char* uniform);
	int getAttributeLocation(const char* uniform);

	void setCustomUniform(std::string customUniform);

	void buildShadersFromSource(std::string shaderSource);
	void bind() const;
	void unbind() const;

	struct UniformsUse {
		int World = -1;
		int View = -1;
		int WorldView = -1;
		int WorldInverseTranspose = -1;
		int WorldViewInverseTranspose = -1;
		int Projection = -1;
		int WorldViewProjection = -1;
	};

	struct AttributesUse {
		int Position = -1;
		int Normal = -1;
		int Tangent = -1;
		int Bitangent = -1;
		int TextureCoord0 = -1;
		int TextureCoord1 = -1;
		int TextureCoord2 = -1;
	};

	static ShaderProgram& getCurrentBound();

private:	
	void buildVertShaderFromSource(const char* vertShaderSource);
	void buildFragShaderFromSource(const char* fragShaderSource);
	uint compileShaderFromSource(uint shaderType, const char* source);
	void link();

	uint mShaderProgram;
	uint mVertShader;
	uint mFragShader;

	std::string VERSION;

	std::string PRECODE_VERTEX;
	std::string PRECODE_FRAGMENT;
	
	std::string ATTRIBUTE_VERTEX_POSITION;
	std::string ATTRIBUTE_NORMAL_POSITION;
	std::string ATTRIBUTE_TEXTURECOORD0;
	std::string ATTRIBUTE_TEXTURECOORD1;
	std::string ATTRIBUTE_TEXTURECOORD2;

	UniformsUse mUniformsUse;
	AttributesUse mAttributesUse;
};

