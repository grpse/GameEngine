#pragma once

const char* ParticleShaderStr = R"(
    #BEGIN VERTEXSHADER																		
    out vec2 Texcoord;
    void main() {
        Texcoord = (VertexPosition_ModelSpace + vec3(0.5, 0.5, 0)).xy;
        Texcoord.y = 1.0 - Texcoord.y;																		
        gl_Position = Projection * WorldView * vec4(VertexPosition_ModelSpace, 1); 			
        //gl_Position = vec4(VertexPosition_ModelSpace, 1); 			
    }																						
    #END VERTEXSHADER
                                                                
    #BEGIN FRAGMENTSHADER		
    in vec2 Texcoord;
    uniform sampler2D tex;
    void main() {																			
        gl_FragColor = texture(tex, Texcoord);// * vec4(1); 														
    }																						
    #END FRAGMENTSHADER																	
)";