#include <GL/glew.h>
#include "Renderer.h"
#include "MeshRenderer.h"
#include "MeshShaderSource.h"
#include "Mesh.h"
#include "Actor.h"
#include "Light.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "GLErrorHandling.h"
#include "Material.h"
#include "ShadowRenderer.h"
#include "LinearMath.h"
#include "Transform.h"
#include "FrameBuffer.h"

MeshRenderer::MeshRenderer()
{
	mShadowRenderer = new ShadowRenderer();
	ShaderProgram Shader;
	Shader.useVertexAttribute();
	Shader.useNormalAttribute();
	Shader.useTextureCoord0Attribute();

	Shader.useProjectionMatrix();
	Shader.useWorldViewMatrix();
	Shader.useWorldViewProjectionMatrix();
	Shader.useWorldMatrix();
	Shader.useViewMatrix();

	Shader.buildShadersFromSource(MeshShaderSource);
	mMaterial.setShaderProgram(Shader);

	mShouldCastShadow = false;
	mShouldReceiveShadow = false;
}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::setMesh(const Mesh& mesh)
{
	mesh.markAsCopy();
	mCurrentMesh = mesh;
}

void MeshRenderer::setReceiveShadow(const bool shouldReceiveShadow)
{
	mShouldReceiveShadow = shouldReceiveShadow;
}

void MeshRenderer::setCastShadow(const bool shouldCastShadow)
{
	mShouldCastShadow = shouldCastShadow;
}

void MeshRenderer::preRender(const Camera & camera, const Light * lights, uint lightsCount, const Actor & actor, const Renderer & renderer) const
{
	/*
	mShadowRenderer->getShadowBuffer().bind();
	for (uint lightIndex = 0; lightIndex < lightsCount; lightIndex++)
	{
		//mShadowRenderer->renderShadowMap(camera, *mCurrentMesh, actor.transform, lights[lightIndex], renderer);
	}
	mShadowRenderer->getShadowBuffer().unbind();
	*/
}

void MeshRenderer::render(const Camera & camera, const Light * lights, uint lightsCount, const Actor & actor, const Renderer & renderer) const
{
	prepare(renderer);

	//TODO: render phong model (maybe), lit without shadows.
	const auto& vao = mCurrentMesh.getVertexArray();
	const auto& ibo = mCurrentMesh.getIndexBuffer();

	const Matrix4& World = actor.transform.getWorldMatrix();
	const Matrix4& View = camera.getViewMatrix();
	const Matrix4 WorldView = View * World;
	const Matrix4 WorldViewProjection = camera.getProjectionMatrix() * WorldView;

	mMaterial.getShaderProgram().setWorldViewProjectionMatrix(WorldViewProjection);
	mMaterial.getShaderProgram().setWorldViewMatrix(WorldView);
	mMaterial.getShaderProgram().setWorldMatrix(World);
	mMaterial.getShaderProgram().setViewMatrix(View);

	renderer.render(vao, ibo);

	finishRendering(renderer);
}

void MeshRenderer::postRender(const Camera & camera, const Light * lights, uint lightsCount, const Actor & actor, const Renderer & renderer) const
{
	//renderer.enableDepthTest();
	//renderer.enableBlend();
	// TODO: apply only shadows on this model using blending
	// TODO: set right blend function
	//renderer.setBlendSrcAlpha_One();

	/*
	for (uint lightIndex = 0; lightIndex < lightsCount; lightIndex++)
	{
		//mShadowRenderer->renderAdditiveShadow(camera, *mCurrentMesh, actor.transform, lights[lightIndex], renderer);
	}
	*/
}

Renderable::QueueType MeshRenderer::getRenderQueue() const 
{
	//	return mMaterial->getQueueType();
	return Renderable::QueueType::Opaque;
}

void MeshRenderer::setMaterial(const Material & material)
{
	mMaterial = material;
}

const Material & MeshRenderer::getMaterial() const
{
	return mMaterial;
}

bool MeshRenderer::hasPrePassStep() const
{
	return mShouldCastShadow;
}

bool MeshRenderer::hasPostPassStep() const
{
	return mShouldReceiveShadow;
}

void MeshRenderer::prepare(const Renderer& renderer) const
{
	mMaterial.use();
	renderer.enableDepthTest();
	//renderer.enableCullFace();
	//renderer.cullBackFace();
}

void MeshRenderer::finishRendering(const Renderer& renderer) const
{
	//renderer.disableCullFace();
	//renderer.disableDepthTest();
}