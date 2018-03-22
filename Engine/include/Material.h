#pragma once
#include "Typedefs.h"
#include "LinearMath.h"
#include "Renderable.h"
#include <map>
#include <tuple>
#include "ShaderProgram.h"
class Camera;
class Light;
class Texture2D;

class Material {

public:
	Material();
	~Material();
	ShaderProgram & getShaderProgram();
	Renderable::QueueType getQueueType() const;
	void setShader(const std::string& shaderSource);
	void setShaderProgram(const ShaderProgram& shaderProgram);
	void setInt(const std::string& name, int i);
	void setFloat(const std::string& name, float f);
	void setVector4(const std::string& name, const Vector4& v);
	void setVector3(const std::string& name, const Vector3& v);
	void setVector2(const std::string& name, const Vector2& v);
	void setMatrix4(const std::string& name, const Matrix4& m);
	void setTexture2D(const std::string& name, const Texture2D& t);
	void setLights(const Light* lights, uint lightsCount);
	void use();

	static void setLightsVectorNames(uint lightsCount);

	struct LightUniformNames
	{
		std::string intensityName;
		std::string directionName;
		std::string positionName;
		std::string colorName;
		std::string typeName;
	};

private:
	ShaderProgram mShader;
	Renderable::QueueType mQueueType;

	std::map<std::string, std::pair<uint, int> > mIntValues;
	std::map<std::string, std::pair<uint, float> > mFloatValues;
	std::map<std::string, std::pair<uint, Vector4> > mVector4Values;
	std::map<std::string, std::pair<uint, Vector3> > mVector3Values;
	std::map<std::string, std::pair<uint, Vector2> > mVector2Values;
	std::map<std::string, std::pair<uint, Matrix4> > mMatrix4Values;
	std::map<std::string, std::pair<uint, Texture2D> > mTexture2DValues;

	std::map<std::string, int> mUniformsMap;

	uint getUniformLocation(const std::string& name);
};