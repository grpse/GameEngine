#pragma once

const char* PhongSoftwareShading = R"(

#vertex vertProgram
#fragment fragProgram

Vector4 vertProgram()
{
	PASS_POSITION = POSITION;
	PASS_NORMAL = NORMAL;
	return vec4(POSITION, 1);
}

Vector4 fragProgram() {

	vec3 WorldEyePosition = VIEW[3].xyz;
	vec3 WorldVertexPosition = PASS_POSITION; // TODO: transform WORLD VIEW PROJECTED POSITION into World position???

	vec3 N = normalize(PASS_NORMAL); 
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


const char* GouraudSoftwareADShading = R"(


#vertex vertProgram
#fragment fragProgram

#begin vertex_variables
out vec4 GouraudSoftwareADShadingColor;
#end vertex_variables

Vector4 vertProgram()
{
	vec3 WorldEyePosition = VIEW[3].xyz;
	vec3 WorldVertexPosition = POSITION; // TODO: transform WORLD VIEW PROJECTED POSITION into World position???

	vec3 N = normalize(NORMAL); 
	vec3 E = normalize(WorldEyePosition - WorldVertexPosition);
	vec3 L = normalize(-light.direction);
	vec3 H = normalize(E + L);

	//calculate the diffuse and specular contributions
	float diff = max(0, dot(N, L));

	//output diffuse
	vec4 diffColor = DiffuseColor * diff * light.color;

	GouraudSoftwareADShadingColor = vec4((AmbientColor + diffColor).rgb, 1);

	return vec4(POSITION, 1);
}

#begin fragment_variables

in vec4 GouraudSoftwareADShadingColor;

#end fragment_variables

Vector4 fragProgram() {

	return GouraudSoftwareADShadingColor;
}
)";


const char* GouraudSoftwareADSShading = R"(


#vertex vertProgram
#fragment fragProgram

#begin vertex_variables
out vec4 GouraudSoftwareADSShadingColor;
#end vertex_variables

Vector4 vertProgram()
{
	vec3 WorldEyePosition = VIEW[3].xyz;
	vec3 WorldVertexPosition = POSITION; // TODO: transform WORLD VIEW PROJECTED POSITION into World position???

	vec3 N = normalize(NORMAL); 
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
	GouraudSoftwareADSShadingColor = vec4((AmbientColor + specColor + diffColor).rgb, 1);

	return vec4(POSITION, 1);
}

#begin fragment_variables

in vec4 GouraudSoftwareADSShadingColor;

#end fragment_variables

Vector4 fragProgram() {

	return GouraudSoftwareADSShadingColor;
}

)";