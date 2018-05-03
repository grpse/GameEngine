#pragma once

const char* PhongShading = R"(

#vertex vertProgram
#fragment fragProgram

Vector4 vertProgram()
{
	PASS_POSITION = POSITION;
	PASS_NORMAL = NORMAL;
	return WORLDVIEWPROJECTION * vec4(POSITION, 1);
}

Vector4 fragProgram() {

	vec3 WorldEyePosition = VIEW[3].xyz;
	vec3 WorldVertexPosition = (WORLD * vec4(PASS_POSITION, 1)).xyz;

	vec3 N = normalize(WORLDINVERSETRANSPOSE * vec4(PASS_NORMAL, 1)).xyz;
	vec3 E = normalize(WorldEyePosition - WorldVertexPosition);
	vec3 L = normalize(-light.direction);
	vec3 H = normalize(E + L);

	//calculate the diffuse and specular contributions
	float diff = max(0, dot(N, L));
	float spec = pow(max(0, dot(N, H)), light.intensity);
	
	if(diff <= 0) {
		spec = 0;
	}

	//output diffuse
	vec4 diffColor = DiffuseColor * diff * light.color;

	//output specular
	vec4 specColor = SpecularColor * light.color * spec;
	return vec4((AmbientColor + specColor + diffColor).rgb, 1);
}

)";

const char* GouraudADS = R"(

#vertex vertProgram
#fragment fragProgram

// TODO: remove uniform duplications to avoid redefinitions errors
// TODO: remove in/out duplications to avoid redefinitions errors
// TODO: in/out definition could be preprocessed to create on vertex -> fragment context

#begin vertex_variables
out vec4 GouraudADSColor;
#end vertex_variables

Vector4 vertProgram()
{
	vec3 WorldEyePosition = VIEW[3].xyz;
	vec3 WorldVertexPosition = (WORLD * vec4(POSITION, 1)).xyz;

	vec3 N = normalize(WORLDINVERSETRANSPOSE * vec4(NORMAL, 1)).xyz;
	vec3 E = normalize(WorldEyePosition - WorldVertexPosition);
	vec3 L = normalize(-light.direction);
	vec3 H = normalize(E + L);

	//calculate the diffuse and specular contributions
	float diff = max(0, dot(N, L));
	float spec = pow(max(0, dot(N, H)), light.intensity);
	
	if(diff <= 0) {
		spec = 0;
	}

	//output diffuse
	vec4 diffColor = DiffuseColor * diff * light.color;

	//output specular
	vec4 specColor = SpecularColor * light.color * spec;
	GouraudADSColor = vec4((AmbientColor + specColor + diffColor).rgb, 1);

	return WORLDVIEWPROJECTION * vec4(POSITION, 1);
}

#begin fragment_variables
in vec4 GouraudADSColor;
#end fragment_variables

Vector4 fragProgram() {
	return GouraudADSColor;
}

)";


const char* GouraudAD = R"(

#vertex vertProgram
#fragment fragProgram


#begin vertex_variables
out vec4 GouraudADColor;
#end vertex_variables

Vector4 vertProgram()
{
	vec3 WorldEyePosition = VIEW[3].xyz;
	vec3 WorldVertexPosition = (WORLD * vec4(POSITION, 1)).xyz;

	vec3 N = normalize(WORLDINVERSETRANSPOSE * vec4(NORMAL, 1)).xyz;
	vec3 E = normalize(WorldEyePosition - WorldVertexPosition);
	vec3 L = normalize(-light.direction);
	vec3 H = normalize(E + L);

	//calculate the diffuse 
	float diff = max(0, dot(N, L));

	//output diffuse
	vec4 diffColor = DiffuseColor * diff * light.color;

	GouraudADColor = vec4((AmbientColor + diffColor).rgb, 1);

	return WORLDVIEWPROJECTION * vec4(POSITION, 1);
}

#begin fragment_variables
in vec4 GouraudADColor;
#end fragment_variables

Vector4 fragProgram() {
	return GouraudADColor;
}

)";