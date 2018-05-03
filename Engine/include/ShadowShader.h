#pragma once
char ShadowShader[] = R"(

#queue Opaque

#vertex vertProgram
#fragment fragProgram

#begin uniforms
Matrix4 depthMVP;
#end uniforms

Vector4 vertProgram() {
	return depthMVP * vec4(POSITION, 1);
}

Vector4 fragProgram() {
	return Vector4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z);
}

)";

char ShadowMapShaderAdditive[] = R"(

#queue Opaque

#vertex vertProgram
#fragment fragProgram

#begin uniforms
uniform mat4 DepthMVPBias;
uniform float ShadowIntensity;
uniform sampler2D ShadowMap;
#end uniforms

#begin vertex_variables
out vec4 ShadowCoord;
#end vertex_variables

Vector4 vertProgram()
{
	ShadowCoord = DepthMVPBias * vec4(POSITION, 1);
	return WORLDVIEWPROJECTION * vec4(POSITION, 1);	
}

#begin fragment_variables
in vec4 ShadowCoord;
#end fragment_variables

Vector4 fragProgram() 
{
	float visibility = 1.0;
	float bias = 0.005;

	if (texture(ShadowMap, ShadowCoord.xy).z  <  ShadowCoord.z - bias) visibility = 0.1;
	
	return vec4(1, 1, 1, 1) * visibility * ShadowIntensity;
}

)";
