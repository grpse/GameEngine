#include "BillboardRenderer.h"
#include <iostream>
#include "BillboardShader.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "LinearMath.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Rect.h"
#include "Camera.h"
#include "AttributesNamesDefines.h"

struct BillboardVertex {
	Vector3 position;
	Vector2 uv;
};

BillboardVertex quad[] = {
	{ Vector3(-1, -1, 0), Vector2(0, 0) },
	{ Vector3(1, -1, 0), Vector2(1, 0) },
	{ Vector3(1,  1, 0), Vector2(1, 1) },
	{ Vector3(-1,  1, 0), Vector2(0, 1) }
};

BillboardRenderer::BillboardRenderer()
{
	mShader.addProgram(BillboardShader);

	VertexBufferLayout layout;
	layout.pushFloat(3, POSITION);
	layout.pushFloat(2, TEXCOORD0);

	mVAO.createVertexBuffer<BillboardVertex>(quad, sizeof(quad), layout);
}

void BillboardRenderer::setup()
{
	mShader.bind();
	mTextureLocation = mShader.getUniformLocation("billboardTexture");
	mDisplacementLocation = mShader.getUniformLocation("displacement");
	mWidthHeightLocation = mShader.getUniformLocation("widthHeight");
	mShader.unbind();
}

void BillboardRenderer::render(const Texture2D& texture, const Rect& rect, const Renderer& renderer)
{
	renderer.disableBlend();
	mShader.bind();

	mShader.setUniform(mTextureLocation, texture);
	mShader.setUniform(mDisplacementLocation, Vector2(rect.x, rect.y));
	mShader.setUniform(mWidthHeightLocation, Vector2(rect.width, rect.height));
	renderer.render(mVAO, 0, 4);

	mShader.unbind();
	renderer.enableBlend();
}