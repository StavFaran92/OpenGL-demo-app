#version 330 core

uniform samplerCube skybox;
in vec3 TexCoords3D;
out vec4 color;

void main()
{    
    color = vec4(texture(skybox, TexCoords3D).xyz, 1.0);
}