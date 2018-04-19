#pragma once

const char* MeshSoftwareShaderSource = R"(

#vertex vertProgram
#fragment fragProgram

#begin vertex_variables
out vec3 PASS_POSITION_SOFTWARE;
out vec3 PASS_NORMAL_SOFTWARE;
#end vertex_variables

Vector4 vertProgram()
{
	PASS_POSITION = POSITION;
	PASS_NORMAL = NORMAL;
	return vec4(POSITION, 1);
}

#begin fragment_variables

#define MAX_LIGHTS_COUNT 10

struct Light {
	vec3 position;
	vec3 direction;
	vec4 color;
	float intensity;
	int type;
};

in vec3 PASS_POSITION_SOFTWARE;
in vec3 PASS_NORMAL_SOFTWARE;

//uniform Light LIGHTS[MAX_LIGHTS_COUNT];
//uniform int LIGHTS_COUNT = 0;
uniform Light directional_software;

#end fragment_variables

Vector4 fragProgram() {
	vec4 AmbientLightColor = vec4(0.3, 0.3, 0.3, 1);
	vec4 SpecularColor = vec4(1, 1, 1, 1);
	vec4 DiffuseColor = vec4(1, 1, 1, 1);

	vec3 WorldEyePosition = VIEW[3].xyz;
	vec3 WorldVertexPosition = (WORLD * vec4(PASS_POSITION_SOFTWARE, 1)).xyz;

	vec3 N = normalize(WORLDINVERSETRANSPOSE * vec4(PASS_NORMAL_SOFTWARE, 1)).xyz;
	vec3 E = normalize(WorldEyePosition - WorldVertexPosition);
	vec3 L = normalize(-directional_software.direction);
	vec3 H = normalize(E + L);

	//calculate the diffuse and specular contributions
	float diff = max(0, dot(N, L));
	float spec = pow(max(0, dot(N, H)), directional_software.intensity);
	
	if(diff <= 0) {
		spec = 0;
	}

	//output diffuse
	vec4 diffColor = DiffuseColor * diff * directional_software.color;

	//output specular
	vec4 specColor = SpecularColor * directional_software.color * spec;
	return vec4((AmbientLightColor + specColor + diffColor).rgb, 1);
}

)";