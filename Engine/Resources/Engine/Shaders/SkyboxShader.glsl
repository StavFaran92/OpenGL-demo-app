#vert

#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords3D;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords3D = aPos;
    mat4 rotView = mat4(mat3(view)); // remove translation from the view matrix
    vec4 clipPos = projection * rotView * vec4(aPos, 1.0);
    gl_Position = clipPos.xyww;
}  

#frag

#version 330 core

uniform samplerCube skybox;
in vec3 TexCoords3D;
out vec4 color;

void main()
{    
    color = vec4(texture(skybox, TexCoords3D).xyz, 1.0);
}