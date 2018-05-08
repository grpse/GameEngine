#pragma once
#include "Typedefs.h"
#include "CubeMap.h"
#include "Renderer.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "VertexArray.h"
#include "OSExport.h"

class ENGINE_API SkyboxRenderer 
{

public:
	SkyboxRenderer();
	SkyboxRenderer(const SkyboxRenderer& other);
	void setShader(const ShaderProgram& shader);
	void render(const Camera& camera, const CubeMap& SkyMap, const Renderer& renderer) const;

private:
	mutable ShaderProgram mShader;
	VertexArray mVAO;
};