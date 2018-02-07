#pragma once
#include "Renderer.h"

class RendererOpenGL : public Renderer {

public:
	
	virtual void clearColorAndDepth() const override;

	virtual void render(const VertexArray & vao, const IndexBuffer & ibo) const override;

	virtual void cullBackFace() const override;

	virtual void enableCullFace() const override;

	virtual void disableCullFace() const override;

	virtual void enableDepthTest() const override;

	virtual void disableDepthTest() const override;

	virtual void enableBlend() const override;

	virtual void setDepthMask() const override;

	virtual void unsetDepthMask() const override;

	virtual void setBlendSrcAlpha_OneMinusSrcColor() const override;

};