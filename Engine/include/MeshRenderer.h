#pragma once
#include "ShaderProgram.h"
#include "LinearMath.h"
#include "OSExport.h"

class Renderer;
class Mesh;
class Camera;
class Transform;
class Light;

class ENGINE_API MeshRenderer {

public:
	MeshRenderer();
	~MeshRenderer();

	void setShader(const ShaderProgram& shader);
	void render(const Camera& camera, const Mesh& mesh, const Transform& transform, const Renderer& renderer) const;

private:
	mutable ShaderProgram mShader;
};
