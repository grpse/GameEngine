#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Rect.h"
#include "LinearMath.h"
#include "OSExport.h"

class Mesh;

class ENGINE_API Renderer {

public:

	enum class Mode {
		Points = 0x0000, //GL_POINTS
		Lines = 0x0001, //GL_LINES,
		Triangles = 0x0004, //GL_TRIANGLES,
		Quads = 0x0007, //GL_QUADS,
	};

	Renderer();
	virtual void setRenderMode(Renderer::Mode mode);
	virtual void setFrontClockwise() const;
	virtual void setFrontCounterClockwise() const;
	virtual void setClearColor(Color32 color) const;
	virtual void clearColorAndDepth() const;
	virtual void render(const VertexArray& vao, const IndexBuffer& ibo) const;
	virtual void render(const VertexArray& vao, uint startIndex, uint count) const;
	virtual void render(const Mesh& mesh) const;
	virtual bool shouldCullbackFace() const;
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
	virtual void setBlendSrcAlpha_OneMinusSrcColor() const;
	virtual void setBlendSrcAlpha_One() const;

private:
	Mode mMode;
	mutable bool mCullbackFace = false;
};