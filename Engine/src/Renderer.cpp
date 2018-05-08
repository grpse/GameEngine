#include <GL/glew.h>

#include "Renderer.h"
#include "GLErrorHandling.h"
#include "Mesh.h"

Renderer::Renderer()
{
	mMode = Renderer::Mode::Triangles;
}

void Renderer::setRenderMode(Renderer::Mode mode)
{
	mMode = mode;
}

void Renderer::setFrontClockwise() const
{
	GLCall(glFrontFace(GL_CW));
}

void Renderer::setFrontCounterClockwise() const
{
	GLCall(glFrontFace(GL_CCW));
}

void Renderer::setClearColor(Color32 color) const
{
	GLCall(glClearColor(color.r, color.g, color.b, color.a));
}

void Renderer::clearColorAndDepth() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::render(const VertexArray & vao, const IndexBuffer & ibo) const
{
	vao.bind();
	ibo.bind();
	GLCall(glDrawElements((uint)mMode, ibo.getElementCount(), GL_UNSIGNED_INT, (const void*)0));
	ibo.unbind();
	vao.unbind();
}

void Renderer::render(const VertexArray& vao, uint startIndex, uint count) const
{
	vao.bind();
	//GLCall(glDrawArrays((uint)mMode, startIndex, count));
	glDrawArrays((uint)mMode, startIndex, count);
	vao.unbind();
}

void Renderer::render(const Mesh& mesh) const
{
	if (mesh.mIsIndexed)
	{
		render(mesh.mVertexArray, mesh.mIndexBuffer);
	}
	else
	{
		render(mesh.mVertexArray, mesh.mIndexStart, mesh.mIndexEnd);
	}
}

bool Renderer::shouldCullbackFace() const
{
	return mCullbackFace;
}

void Renderer::cullBackFace() const
{
	GLCall(glCullFace(GL_BACK));
	mCullbackFace = true;
}

void Renderer::cullFrontFace() const
{
	GLCall(glCullFace(GL_FRONT));
}

void Renderer::enableCullFace() const
{
	GLCall(glEnable(GL_CULL_FACE));

}

void Renderer::disableCullFace() const
{
	GLCall(glDisable(GL_CULL_FACE));
	mCullbackFace = false;
}

void Renderer::enableDepthTest() const
{
	GLCall(glEnable(GL_DEPTH_TEST));
}

void Renderer::disableDepthTest() const
{
	GLCall(glDisable(GL_DEPTH_TEST));
}

void Renderer::enableBlend() const
{
	GLCall(glEnable(GL_BLEND));
}

void Renderer::disableBlend() const
{
	GLCall(glDisable(GL_BLEND));
}

void Renderer::setViewport(const Rect & viewport) const
{
	GLCall(glViewport(viewport.x, viewport.y, viewport.width, viewport.height));
}

void Renderer::setDepthMask() const
{
	GLCall(glDepthMask(GL_TRUE));
}

void Renderer::unsetDepthMask() const
{
	GLCall(glDepthMask(GL_FALSE));
}

void Renderer::setBlendSrcAlpha_OneMinusSrcColor() const
{
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR));
}

void Renderer::setBlendSrcAlpha_One() const
{
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE));
}
