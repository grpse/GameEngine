#pragma once

const char* MeshShaderSource = R"(

#BEGIN VERTEXSHADER

void main()
{
	gl_Position = Projection * WorldView * vec4(VertexPosition_ModelSpace, 1);
}

#END VERTEXSHADER

#BEGIN FRAGMENTSHADER

void main()
{
	gl_FragColor = vec4(1, 1, 1, 1);
}

#END FRAGMENTSHADER

)";