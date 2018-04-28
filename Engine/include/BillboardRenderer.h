#pragma once
#include "ShaderProgram.h"
#include "Typedefs.h"
#include "VertexArray.h"

class Texture2D;
struct Rect;
class Renderer;

class BillboardRenderer {
public:
	
	BillboardRenderer();
	void setup();
	void render(const Texture2D& texture, const Rect& rect, const Renderer& renderer);

private:
	ShaderProgram mShader;
	uint mTextureLocation;
	uint mDisplacementLocation;
	uint mWidthHeightLocation;
	VertexArray mVAO;
};