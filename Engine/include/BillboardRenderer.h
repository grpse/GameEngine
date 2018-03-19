#pragma once
#include "Renderer.h"
#include "Texture2D.h"
#include "Rect.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "Typedefs.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "LinearMath.h"

const char* BillboardShader = R"(

#BEGIN VERTEXSHADER

uniform vec2 displacement;
uniform vec2 widthHeight;

out vec2 TextureCoord0_pass;

void main() {
	vec2 VertexPositionTransformed = VertexPosition_ModelSpace.xy;
	VertexPositionTransformed.x *= widthHeight.x;
	VertexPositionTransformed.y *= widthHeight.y;
	VertexPositionTransformed.x += displacement.x;
	VertexPositionTransformed.y += displacement.y;

	gl_Position = vec4(VertexPositionTransformed, 0, 1);
	TextureCoord0_pass = TextureCoord0;
}

#END VERTEXSHADER

#BEGIN FRAGMENTSHADER

in vec2 TextureCoord0_pass;

uniform sampler2D billboardTexture;

void main() {
	gl_FragColor = texture(billboardTexture, TextureCoord0_pass);
}
#END FRAGMENTSHADER	

)";

struct BillboardVertex {
	Vector3 position;
	Vector2 uv;
};

static BillboardVertex quad[] = {
	{Vector3(-1, -1, 0), Vector2(0, 0) },
	{Vector3( 1, -1, 0), Vector2(1, 0) },
	{Vector3( 1,  1, 0), Vector2(1, 1) },
	{Vector3(-1,  1, 0), Vector2(0, 1) }
};

class BillboardRenderer {
public:
	
	explicit BillboardRenderer()
	{
		mShader.useVertexAttribute();
		mShader.useTextureCoord0Attribute();
		mShader.useProjectionMatrix();
		mShader.buildShadersFromSource(BillboardShader);

		mShader.start();
		mTextureLocation = mShader.getUniformLocation("billboardTexture");
		mDisplacementLocation = mShader.getUniformLocation("displacement");
		mWidthHeightLocation = mShader.getUniformLocation("widthHeight");
		mShader.stop();
		
		VertexBuffer vertexBuffer(quad, sizeof(quad));
		VertexBufferLayout layout;
		layout.pushFloat(3);
		layout.pushFloat(2);

		mVAO.generateBuffer();
		mVAO.setVertexBuffer(vertexBuffer, layout);
	}

	void render(const Texture2D& texture, const Rect& rect, const Renderer& renderer)
	{
		renderer.disableBlend();
		texture.start();
		mShader.start();

		mShader.setUniform(mTextureLocation, (int)0);
		mShader.setUniform(mDisplacementLocation, Vector2(rect.x, rect.y));
		mShader.setUniform(mWidthHeightLocation, Vector2(rect.width, rect.height));

		mVAO.bind();
		renderer.renderQuad(0, 4);
		mVAO.unbind();

		texture.stop();
		mShader.stop();
		renderer.enableBlend();
	}

private:
	ShaderProgram mShader;
	uint mTextureLocation;
	uint mDisplacementLocation;
	uint mWidthHeightLocation;
	VertexArray mVAO;
};