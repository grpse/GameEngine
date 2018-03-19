#include "Material.h"

std::vector<Material::LightUniformNames> LightsNames;

ShaderProgram& Material::getShaderProgram()
{
	return mShader;
}

Renderable::QueueType Material::getQueueType() const
{
	return mQueueType;
}

void Material::setShader(const std::string & shaderSource)
{
	mShader.buildShadersFromSource(shaderSource);
}

void Material::setShaderProgram(const ShaderProgram& shaderProgram)
{
	mShader = shaderProgram;
}

void Material::setInt(const std::string & name, int i)
{
	mIntValues[name] = { getUniformLocation(name), i };
}

void Material::setFloat(const std::string & name, float f)
{
	mFloatValues[name] = { getUniformLocation(name), f };
}

void Material::setVector4(const std::string& name, const Vector4& v)
{
	mVector4Values[name] = { getUniformLocation(name), v };
}

void Material::setVector3(const std::string & name, const Vector3 & v)
{
	mVector3Values[name] = { getUniformLocation(name), v };
}

void Material::setVector2(const std::string & name, const Vector2 & v)
{
	mVector2Values[name] = { getUniformLocation(name), v };
}

void Material::setMatrix4(const std::string & name, const Matrix4 & m)
{
	mMatrix4Values[name] = { getUniformLocation(name), m };
}

void Material::setTexture2D(const std::string & name, const Texture2D & t)
{
	mTexture2DValues[name] = { getUniformLocation(name), t };
}

void Material::setLights(const Light* lights, uint lightsCount)
{
	mIntValues["LIGHTS_COUNT"] = { getUniformLocation("LIGHTS_COUNT"), lightsCount };

	for (uint lightIndex = 0; lightIndex < lightsCount; lightIndex++)
	{
		const LightUniformNames& lightUniformName = LightsNames[lightIndex];

		mFloatValues[lightUniformName.intensityName] = { getUniformLocation(lightUniformName.intensityName), lights[lightIndex].intensity };
		mVector3Values[lightUniformName.directionName] = { getUniformLocation(lightUniformName.directionName), lights[lightIndex].direction };
		mVector3Values[lightUniformName.positionName] = { getUniformLocation(lightUniformName.positionName), lights[lightIndex].position };
		mVector4Values[lightUniformName.colorName] = { getUniformLocation(lightUniformName.colorName), lights[lightIndex].color };
		mIntValues[lightUniformName.typeName] = { getUniformLocation(lightUniformName.typeName), (int)lights[lightIndex].type };
	}
}

void Material::use()
{
	mShader.start();

	uint index = 0;
	
	for (auto& t : mTexture2DValues)
	{
		uint uniformLocation = t.second.first;
		Texture2D& texture = t.second.second;
		texture.setIndex(index++);
		texture.start();
		mShader.setUniform(uniformLocation, (int)index);
	}

	for (auto& f : mFloatValues)
	{
		uint uniformLocation = f.second.first;
		float value = f.second.second;
		mShader.setUniform(uniformLocation, value);
	}

	for (auto& i : mIntValues)
	{
		uint uniformLocation = i.second.first;
		int value = i.second.second;
		mShader.setUniform(uniformLocation, (int)value);
	}

	for (auto& v3 : mVector3Values)
	{
		uint uniformLocation = v3.second.first;
		Vector3& value = v3.second.second;
		mShader.setUniform(uniformLocation, value);
	}

	for (auto& v2 : mVector2Values)
	{
		uint uniformLocation = v2.second.first;
		Vector2& value = v2.second.second;
		mShader.setUniform(uniformLocation, value);
	}

	for (auto& m4 : mMatrix4Values)
	{
		uint uniformLocation = m4.second.first;
		Matrix4& value = m4.second.second;
		mShader.setUniform(uniformLocation, value);
	}
}

uint Material::getUniformLocation(const std::string& name)
{
	int uniformLocation = mUniformsMap[name];
	if (uniformLocation < 0)
	{
		mShader.start();
		const char* uniformName = name.c_str();
		uniformLocation = mShader.getUniformLocation(uniformName);
		mUniformsMap[name] = uniformLocation;
	}

	return uniformLocation;
}

void Material::setLightsVectorNames(uint lightsCount)
{
	if (lightsCount > LightsNames.size())
	{
		uint lightCountStart = lightsCount - LightsNames.size();
		for (uint lightIndex = lightCountStart; lightIndex < lightsCount; lightIndex++)
		{
			const std::string LightIntensityName = "LIGHTS[" + std::to_string(lightIndex) + "].intensity";
			const std::string LightDirectionName = "LIGHTS[" + std::to_string(lightIndex) + "].direction";
			const std::string LightPositionName = "LIGHTS[" + std::to_string(lightIndex) + "].position";
			const std::string LightColorName = "LIGHTS[" + std::to_string(lightIndex) + "].color";
			const std::string LightTypeName = "LIGHTS[" + std::to_string(lightIndex) + "].type";
			LightsNames.push_back(
				{
					LightIntensityName,
					LightDirectionName,
					LightPositionName,
					LightColorName,
					LightTypeName,
				}
			);
		}
	}
}