#pragma once

const char* MeshShaderSource = R"(

#BEGIN VERTEXSHADER

out vec3 VertexPosition_WorldSpace;
out vec3 VertexNormal_WorldSpace;
out vec3 Eye_WorldSpace;

void main()
{
	VertexPosition_WorldSpace = (vec4(VertexPosition_ModelSpace, 1) * World).xyz;
	VertexNormal_WorldSpace = (vec4(VertexNormal_ModelSpace, 1) * World).xyz;
	Eye_WorldSpace = View[3].xyz;
	gl_Position = Projection * WorldView * vec4(VertexPosition_ModelSpace, 1);	
}

#END VERTEXSHADER

#BEGIN FRAGMENTSHADER

#define MAX_LIGHTS_COUNT 10

struct Light {
	vec3 position;
	vec3 direction;
	vec4 color;
	float intensity;
	int type;
};

in vec3 VertexPosition_WorldSpace;
in vec3 VertexNormal_WorldSpace;
in vec3 Eye_WorldSpace;

uniform Light LIGHTS[MAX_LIGHTS_COUNT];
uniform int LIGHTS_COUNT = 0;

vec4 CalculateDirectionalDiffuseColor(Light light) {
	float diffuseFactor = max(0, dot(-light.direction, VertexNormal_WorldSpace));
	return light.color * light.intensity * diffuseFactor;
}

vec4 CalculateSpecularColor(Light light) {
	vec3 E = normalize(Eye_WorldSpace - VertexPosition_WorldSpace);
	vec3 R = normalize(reflect(light.direction, VertexNormal_WorldSpace));

	float specularFactor = pow(max(0 , dot(E, R)), light.intensity);

	return light.color * light.intensity * specularFactor;
}

void main() {

	//calculate the diffuse and specular contributions	
	// TODO: External set AmbienteLightColor
	vec4 AmbientLightColor = vec4(0.3, 0.3, 0.3, 1);
	vec4 SpecularColor = vec4(0, 0, 0, 0);
	vec4 DiffuseColor = vec4(0, 0, 0, 0);

	for (int i = 0; i < LIGHTS_COUNT; i++)
	{
		SpecularColor += CalculateSpecularColor(LIGHTS[i]);
		DiffuseColor += CalculateDiffuseColor(LIGHTS[i]);
	}

	gl_FragColor = AmbientLightColor + SpecularColor + DiffuseColor;
}

#END FRAGMENTSHADER

)";