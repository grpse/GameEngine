#pragma once
#include "LinearMath.h"
#include "ShaderProgram.h"
#include <vector>

class Renderer;
class Mesh;
class Camera;
class Transform;
class Light;

class MeshSoftwareRenderer {

public:
	MeshSoftwareRenderer();
	~MeshSoftwareRenderer();

	void setup();
	void render(const Camera& camera, const Mesh& mesh, const Transform& transform, const Light& directional, const Renderer& renderer);
	void setDiffuseColor(Color32 color);
	void usePhong();
	void useGouraudAD();
	void useGouraudADS();
	ShaderProgram& getShaderProgram();

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

	uint mDiffuseColorLocation;

	LightUniforms mLightUniforms;
	
	void prepare(const Renderer& renderer);
	void finishRendering(const Renderer& renderer);
};
