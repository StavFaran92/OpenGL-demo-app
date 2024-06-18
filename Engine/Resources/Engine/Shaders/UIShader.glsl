#vert

#version 330                                                                        
                                                                                    
layout (location = 0) in vec3 aPos;                                             
                                                                                    
//uniform mat4 model;
uniform mat4 projection;  

out vec2 fragPos;
                                                                                    
void main()                                                                         
{
    vec3 pos = aPos;
    fragPos = (pos.xy + 1.0) / 2.0; // Transform from [-1, 1] to [0, 1] range
    gl_Position = projection * vec4(aPos, 1.0);                                       
}

#frag

#version 330

in vec2 fragPos;

uniform sampler2D image;

out vec4 color;

void main()
{
   color = vec4(texture(image, fragPos).rgb, 1.0);
}