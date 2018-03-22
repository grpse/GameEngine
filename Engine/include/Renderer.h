#pragma once
#include "LinearMath.h"

class IndexBuffer;
class VertexArray;
struct Rect;

class Renderer {

public:

	virtual void setClearColor(Color32 color) const;
	virtual void clearColorAndDepth() const;
	virtual void render(const VertexArray& vao, const IndexBuffer& ibo) const;
	virtual void renderQuad(uint startIndex, uint count) const;
	virtual void cullBackFace() const;
	virtual void cullFrontFace() const;
	virtual void enableCullFace() const;
	virtual void disableCullFace() const;
	virtual void enableDepthTest() const;
	virtual void disableDepthTest() const;
	virtual void enableBlend() const;
	virtual void disableBlend() const;
	virtual void setViewport(const Rect& viewport) const;

	virtual void setDepthMask() const;
	virtual void unsetDepthMask() const;
	virtual void setBlendSrcAlpha_One() const;
	virtual void setBlendSrcAlpha_OneMinusSrcColor() const;

};