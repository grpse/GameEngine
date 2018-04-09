#pragma once
#include "Typedefs.h"
#include "CubeMap.h"
#include "Renderer.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "VertexArray.h"

class SkyboxRenderer 
{

public:
	SkyboxRenderer();
	void setCubeMap(const CubeMap& cubeMap);
	void render(const Camera& camera, const Renderer& renderer) const;

private:
	CubeMap mCubeMap;
	mutable ShaderProgram mShader;
	uint mCubeMapLocation;
	uint mCameraPositionLocation;
	VertexArray mVAO;
};