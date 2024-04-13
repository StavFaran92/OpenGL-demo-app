#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 tex;

out vec2 TexCoords;

void main()
{
    TexCoords = tex;

    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
} 