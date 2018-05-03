#pragma once
#include "ShaderProgram.h"
#include "LinearMath.h"

class Renderer;
class Mesh;
class Camera;
class Transform;
class Light;

class MeshRenderer{

public:

	MeshRenderer();
	~MeshRenderer();

	void setup();
	void render(const Camera& camera, const Mesh& mesh, const Transform& transform, const Light& directional, const Renderer& renderer);
	void setDiffuseColor(const Color32& color);
	ShaderProgram& getShaderProgram();
	void usePhong();
	void useGouraudAD();
	void useGouraudADS();

private:
	ShaderProgram mShader;
	ShaderProgram mPhong;
	ShaderProgram mGouraudAD;
	ShaderProgram mGouraudADS;


	struct LightUniforms {
		uint position;
		uint direction;
		uint color;
		uint intensity;
	};

	LightUniforms mLightUniforms;

	uint mDiffuseColorLocation;

	void prepare(const Renderer& renderer);
	void finishRendering(const Renderer& renderer);
};
