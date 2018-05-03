#pragma once
const char* BillboardShader = R"(

#queue Opaque

#vertex vertProgram
#fragment fragProgram

#begin uniforms
uniform vec2 displacement;
uniform vec2 widthHeight;
uniform sampler2D billboardTexture;
#end uniforms

#begin vertex_variables
out vec2 TextureCoord0_pass;
#end vertex_variables

Vector4 vertProgram() {
	vec2 VertexPositionTransformed = POSITION.xy;
	VertexPositionTransformed.x *= widthHeight.x;
	VertexPositionTransformed.y *= widthHeight.y;
	VertexPositionTransformed.x += displacement.x;
	VertexPositionTransformed.y += displacement.y;

	TextureCoord0_pass = TEXCOORD0;
	return vec4(VertexPositionTransformed, 0, 1);
}

#begin fragment_variables
in vec2 TextureCoord0_pass;
#end fragment_variables

Vector4 fragProgram() {
	return texture(billboardTexture, TextureCoord0_pass);
}
)";

