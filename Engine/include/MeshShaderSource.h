#pragma once

const char* MeshShaderSource = R"(

#BEGIN VERTEXSHADER

out vec3 VertexPosition_WorldSpace;
out vec3 VertexNormal_WorldSpace;
out vec3 Eye_WorldSpace;

void main()
{
	VertexPosition_WorldSpace = (World * vec4(VertexPosition_ModelSpace, 1)).xyz;
	Eye_WorldSpace = View[3].xyz;
	VertexNormal_WorldSpace = (World * vec4(VertexNormal_ModelSpace, 1)).xyz;
	gl_Position = Projection * WorldView * vec4(VertexPosition_ModelSpace, 1);
}

#END VERTEXSHADER

#BEGIN FRAGMENTSHADER

in vec3 VertexPosition_WorldSpace;
in vec3 VertexNormal_WorldSpace;
in vec3 Eye_WorldSpace;

vec3 lightDirection = vec3(-1, -1, -1);
float shininess = 0.5;

void main()
{
	vec3 N = normalize(VertexNormal_WorldSpace);
	vec3 E = normalize(Eye_WorldSpace - VertexPosition_WorldSpace); //eye vector
	vec3 L = normalize(-lightDirection.xyz); //light vector
	vec3 H = normalize(E + L); //half angle vector

	//calculate the diffuse and specular contributions
	float  diff = max(0, dot(N, L));
	float  spec = pow(max(0 , dot(N, H)), shininess);
	if(diff <= 0)
	{
		spec = 0;
	}

	vec3 AmbientLightColor = vec3(0, 0, 0);
	vec3 SpecularColor = vec3(1, 1, 1);
	vec3 DiffuseColor = vec3(1, 1, 1);


	gl_FragColor = vec4(AmbientLightColor + SpecularColor * spec + DiffuseColor * diff, 1);
}

#END FRAGMENTSHADER

)";