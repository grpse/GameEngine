#pragma once

const char* ParticleShaderStr = R"(
#vertex vertProgram
#fragment fragProgram

#begin uniforms
uniform sampler2D tex;
#end uniforms

#begin vertex_variables							
out vec2 Texcoord;
#end vertex_variables

Vector4 vertProgram() {
    Texcoord = (POSITION + vec3(0.5, 0.5, 0)).xy;
    Texcoord.y = 1.0 - Texcoord.y;																		
    return PROJECTION * WORLDVIEW * vec4(POSITION, 1);
}

#begin fragment_variables							
in vec2 Texcoord;
#end fragment_variables							

Vector4 fragProgram() {																			
    return texture(tex, Texcoord);
}									

)";