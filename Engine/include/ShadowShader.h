#pragma once
char ShadowShader[] = R"(

#queue Opaque

#begin vertexshader

uniform mat4 depthMVP;

void main() {
	gl_Position = depthMVP * vec4(POSITION, 1);
}

#end vertexshader

#begin fragmentshader

out float fragmentDepth;

void main() {
	fragmentDepth = gl_FragCoord.z;
}

#end fragmentshader

)";

char ShadowMapShaderAdditive[] = R"(

#queue Opaque

#begin vertexshader

uniform mat4 DepthMVPBias;

out vec4 ShadowCoord;

void main()
{
	gl_Position = WORLDVIEWPROJECTION * vec4(POSITION, 1);	
	ShadowCoord = DepthMVPBias * vec4(POSITION, 1);
}

#end vertexshader

#begin fragmentshader

uniform float ShadowIntensity;
uniform sampler2D ShadowMap;

in vec4 ShadowCoord;

void main() 
{
	float visibility = 1.0;
	float bias = 0.005;

	if (texture(ShadowMap, ShadowCoord.xy).z  <  ShadowCoord.z - bias) {
		visibility = 0.1;
	}
	
	gl_FragColor = vec4(1, 1, 1, 1) * visibility * ShadowIntensity;
}

#end fragmentshader

)";
