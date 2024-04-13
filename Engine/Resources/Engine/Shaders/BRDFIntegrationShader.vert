#version 330

layout (location = 0) in vec3 pos;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

// ----- Definitions ----- //

#include ../../../../Engine/Resources/Engine/Shaders/include/defines.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/structs.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/uniforms.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/functions.glsl

void main()
{
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0); 
    TexCoords = aTexCoords;
}