#include <GL/glew.h>
#include "FrameBuffer.h"
#include "GLErrorHandling.h"
#include "Texture2D.h"

FrameBuffer::FrameBuffer()
{
	mRefCount = 0;
}

FrameBuffer::FrameBuffer(const FrameBuffer& fbo)
{
	fbo.mRefCount += 1;

	mFBO = fbo.mFBO;
	mLayout = fbo.mLayout;
	mRefCount = fbo.mRefCount;
}

FrameBuffer::~FrameBuffer()
{
	if (mRefCount == 0)
	{
		GLCall(glDeleteFramebuffers(1, &mFBO));
	}
}

void FrameBuffer::setDrawAttachment(uint attachment)
{
	GLCall(glGenFramebuffers(1, &mFBO));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mFBO));
	GLCall(glDrawBuffer(attachment));
}

Texture2D FrameBuffer::createTextureAttachment(const Texture2DLayout& layout, uint colorAttachmentUnit)
{
	bind();
	Texture2D frameTexture;
	frameTexture.bind();
	frameTexture.loadData(NULL, layout);

	frameTexture.setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	frameTexture.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLCall(glFramebufferTexture(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0 + colorAttachmentUnit,
		frameTexture.getId(),
		0
	));

	frameTexture.unbind();
	unbind();

	return frameTexture;
}

Texture2D FrameBuffer::createDepthTextureAttachment(const Texture2DLayout& layout)
{
	bind();
	Texture2D depthTexture;
	depthTexture.bind();
	depthTexture.loadData(NULL, layout);

	depthTexture.setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	depthTexture.setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	depthTexture.setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	depthTexture.setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//depthTexture.setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//depthTexture.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLCall(glFramebufferTexture(
		GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		depthTexture.getId(),
		0
	));

	depthTexture.unbind();
	unbind();

	return depthTexture;
}

bool FrameBuffer::isComplete() const
{
	// Always check that our framebuffer is ok
	bool frameBufferStatus;
	GLCall(frameBufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	return frameBufferStatus;
}

void FrameBuffer::bind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));//To make sure the texture isn't bound
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mFBO));
	GLCall(glViewport(0, 0, mLayout.width, mLayout.height));
}

void FrameBuffer::unbind() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	// TODO: Reset originals framebuffer
	//glViewport(0, 0, Display.getWidth(), Display.getHeight());
}

void FrameBuffer::setLayout(const FrameBufferLayout& layout)
{
	mLayout = layout;
}