#include <GL/glew.h>
#include <SDL_opengl.h>
#include "GLErrorHandling.h"
#include "RendererOpenGL.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

void RendererOpenGL::clearColorAndDepth() const
{
}

void RendererOpenGL::render(const VertexArray & vao, const IndexBuffer & ibo) const
{
	vao.bind();
	ibo.bind();

	GLCall(glDrawElements(GL_TRIANGLES, ibo.getElementCount(), GL_UNSIGNED_INT, (const void*)0));

	vao.unbind();
}

void RendererOpenGL::cullBackFace() const 
{
	GLCall(glCullFace(GL_BACK));
}

void RendererOpenGL::enableCullFace() const
{
	GLCall(glEnable(GL_CULL_FACE));
}

void RendererOpenGL::disableCullFace() const
{
	GLCall(glDisable(GL_CULL_FACE));
}

void RendererOpenGL::enableDepthTest() const
{
	GLCall(glEnable(GL_DEPTH_TEST));
}

void RendererOpenGL::disableDepthTest() const
{
	GLCall(glDisable(GL_DEPTH_TEST));
}

void RendererOpenGL::enableBlend() const
{
	GLCall(glEnable(GL_BLEND));
}

void RendererOpenGL::setDepthMask() const
{
	GLCall(glDepthMask(GL_TRUE));
}

void RendererOpenGL::unsetDepthMask() const
{
	GLCall(glDepthMask(GL_FALSE));
}

void RendererOpenGL::setBlendSrcAlpha_OneMinusSrcColor() const
{
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR));
}
