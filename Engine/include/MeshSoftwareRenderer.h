#pragma once
#include "LinearMath.h"
#include "ShaderProgram.h"
#include <vector>
#include "OSExport.h"

class Renderer;
class Mesh;
class Camera;
class Transform;
class Light;

class ENGINE_API MeshSoftwareRenderer {

public:
	MeshSoftwareRenderer();
	~MeshSoftwareRenderer();

	void setShader(const ShaderProgram& shader);
	void render(const Camera& camera, const Mesh& mesh, const Transform& transform, const Renderer& renderer) const;

private:
	mutable ShaderProgram mShader;
};
