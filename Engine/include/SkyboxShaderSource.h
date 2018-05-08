#pragma once
const char SkyboxShader[] = R"(

#queue Opaque

#vertex vertProgram
#fragment fragProgram

#begin uniforms
uniform samplerCube Skycube;
#end uniforms

#begin vertex_variables
out vec3 texcoords;
#end vertex_variables

Vector4 vertProgram() {
  texcoords = POSITION;
  return WORLDVIEWPROJECTION * vec4(POSITION + CAMERAPOSITION, 1.0);
}

#begin fragment_variables
in vec3 texcoords;
#end fragment_variables

Vector4 fragProgram() {
	return texture(Skycube, texcoords);
}

)";