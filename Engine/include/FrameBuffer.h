#pragma once
#include "Typedefs.h"

class Texture2D;
struct Texture2DLayout;

struct FrameBufferLayout {
	uint width;
	uint height;
	uint attachment;
};

class FrameBuffer {

public:

	FrameBuffer();
	FrameBuffer(const FrameBuffer& fbo);
	~FrameBuffer();

	void setDrawAttachment(uint attachment);
	Texture2D createTextureAttachment(const Texture2DLayout& layout, uint colorAttachmentUnit);
	Texture2D createDepthTextureAttachment(const Texture2DLayout& layout);

	bool isComplete() const;

	void bind() const;
	void unbind() const;

	void setLayout(const FrameBufferLayout& layout);

private:
	uint mFBO;
	FrameBufferLayout mLayout;
	mutable uint mRefCount;
};