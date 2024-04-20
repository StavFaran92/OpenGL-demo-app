#vert

#version 330                                                                        
                                                                                    
layout (location = 0) in vec3 Position;                                             
                                                                                    
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;                                                                
                                                                                    
void main()                                                                         
{                                                                                   
    gl_Position = projection * view * model * vec4(Position, 1.0);                                       
}

#frag

#version 330

uniform uint objectIndex;
uniform uint drawIndex;

out uvec3 FragColor;

void main()
{
   FragColor = uvec3(objectIndex, drawIndex, gl_PrimitiveID);
}