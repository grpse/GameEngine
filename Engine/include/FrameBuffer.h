#pragma once
#include <GL/glew.h>
#include "GLErrorHandling.h"
#include "Typedefs.h"
#include "Texture2D.h"

struct FrameBufferLayout {
	uint width;
	uint height;
	uint attachment;
};

class FrameBuffer {

public:

	explicit FrameBuffer(uint attachment)
	{
		GLCall(glGenFramebuffers(1, &mFBO));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mFBO));
		GLCall(glDrawBuffer(attachment));

		mRefCount = 0;
	}

	FrameBuffer(const FrameBuffer& fbo)
	{
		fbo.increaseReferences();

		mFBO = fbo.mFBO;
		mLayout = fbo.mLayout;
		mRefCount = fbo.mRefCount;
	}

	~FrameBuffer()
	{
		if (mRefCount == 0)
		{
			GLCall(glDeleteFramebuffers(1, &mFBO));
		}
	}

	Texture2D createTextureAttachment(const Texture2DLayout& layout, uint colorAttachmentUnit)
	{
		bind();
		Texture2D frameTexture;
		frameTexture.start();
		frameTexture.loadData(NULL, layout);

		frameTexture.setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		frameTexture.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		GLCall(glFramebufferTexture(
			GL_FRAMEBUFFER, 
			GL_COLOR_ATTACHMENT0 + colorAttachmentUnit,
			frameTexture.getId(), 
			0
		));

		frameTexture.stop();
		unbind();

		return frameTexture;
	}

	Texture2D createDepthTextureAttachment(const Texture2DLayout& layout)
	{
		bind();
		Texture2D depthTexture;
		depthTexture.start();
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
		
		depthTexture.stop();
		unbind();

		return depthTexture;
	}

	bool isComplete() const
	{
		// Always check that our framebuffer is ok
		bool frameBufferStatus;
		GLCall(frameBufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		return frameBufferStatus;
	}

	void bind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);//To make sure the texture isn't bound
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
		glViewport(0, 0, mLayout.width, mLayout.height);
	}

	void unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// TODO: Reset originals framebuffer
		//glViewport(0, 0, Display.getWidth(), Display.getHeight());
	}

	void setLayout(const FrameBufferLayout& layout) 
	{
		mLayout = layout;
	}

private:
	uint mFBO;
	FrameBufferLayout mLayout;
	mutable uint mRefCount;

	// Increases references to this bound framebuffer to don't delete it
	// on copies passes
	inline void increaseReferences() const
	{
		mRefCount += 1;
	}
};