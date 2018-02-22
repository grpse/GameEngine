#pragma once

const char* MeshShaderSource = R"(

#BEGIN VERTEXSHADER

out vec3 VertexPosition_WorldSpace;
out vec3 VertexNormal_WorldSpace;
out vec3 Eye_WorldSpace;
out vec4 ShadowCoord;

uniform mat4 DepthMVPBias;

void main()
{
	VertexPosition_WorldSpace = (World * vec4(VertexPosition_ModelSpace, 1)).xyz;
	VertexNormal_WorldSpace = (World * vec4(VertexNormal_ModelSpace, 1)).xyz;
	Eye_WorldSpace = View[3].xyz;
	gl_Position = Projection * WorldView * vec4(VertexPosition_ModelSpace, 1);
	
	ShadowCoord = DepthMVPBias * vec4(VertexPosition_ModelSpace, 1);
}

#END VERTEXSHADER

#BEGIN FRAGMENTSHADER

struct DirectionalLight {
	vec3 direction;
	vec4 color;
	float intensity;
};

in vec3 VertexPosition_WorldSpace;
in vec3 VertexNormal_WorldSpace;
in vec3 Eye_WorldSpace;
in float UseShadowMap;
in vec4 ShadowCoord;

float shininess = 0.2;

uniform vec3 LightDirection = vec3(-1, -1, -1);
uniform vec4 LightColor = vec4(1, 1, 1, 1);

uniform sampler2D ShadowMap;

void main() {

	vec3 N = normalize(VertexNormal_WorldSpace);
	vec3 E = normalize(Eye_WorldSpace - VertexPosition_WorldSpace); //eye vector
	vec3 L = normalize(-LightDirection.xyz); //light vector
	vec3 H = normalize(E + L); //half angle vector

	//calculate the diffuse and specular contributions
	float  diff = max(0, dot(N, L));
	float  spec = pow(max(0 , dot(N, H)), shininess);

	if (diff <= 0) spec = 0;

	float visibility = 1.0;
	float bias = 0.005;

	if (texture(ShadowMap, ShadowCoord.xy).z  <  ShadowCoord.z - bias) {
		visibility = 0.1;
	}
	
	vec3 AmbientLightColor = vec3(0.3, 0.3, 0.3);
	vec3 SpecularColor = vec3(1, 1, 1);
	vec3 DiffuseColor = vec3(1, 1, 1);

	gl_FragColor = vec4(
					AmbientLightColor + 
					visibility * SpecularColor * spec + 
					visibility * DiffuseColor * diff, 1);
}

#END FRAGMENTSHADER

)";