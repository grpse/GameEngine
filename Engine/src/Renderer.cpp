#include <GL/glew.h>
#include <SDL_opengl.h>

#include "Renderer.h"
#include "GLErrorHandling.h"

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

	GLCall(glDrawElements(GL_TRIANGLES, ibo.getElementCount(), GL_UNSIGNED_INT, (const void*)0));

	vao.unbind();
}

void Renderer::renderQuad(uint startIndex, uint count) const
{
	GLCall(glDrawArrays(GL_QUADS, startIndex, count));
}

void Renderer::cullBackFace() const
{
	GLCall(glCullFace(GL_BACK));
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

void Renderer::setBlendSrcAlpha_One() const
{
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE));
}

void Renderer::setBlendSrcAlpha_OneMinusSrcColor() const
{
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR));
}
