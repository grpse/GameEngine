#pragma once

char ShadowShader[] = R"(

#BEGIN VERTEXSHADER

uniform mat4 depthMVP;

void main() {
	gl_Position = depthMVP * vec4(VertexPosition_ModelSpace, 1);
}

#END VERTEXSHADER

#BEGIN FRAGMENTSHADER

out float fragmentDepth;

void main() {
	fragmentDepth = gl_FragCoord.z;
}

#END FRAGMENTSHADER

)";

char ShadowMapShaderAdditive[] = R"(

#BEGIN VERTEXSHADER

uniform mat4 DepthMVPBias;

out vec4 ShadowCoord;

void main()
{
	gl_Position = WorldViewProjection * vec4(VertexPosition_ModelSpace, 1);	
	ShadowCoord = DepthMVPBias * vec4(VertexPosition_ModelSpace, 1);
}

#END VERTEXSHADER

#BEGIN FRAGMENTSHADER

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

#END FRAGMENTSHADER

)";

