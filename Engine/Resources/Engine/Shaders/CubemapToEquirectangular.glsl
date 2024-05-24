#vert

#version 330

layout (location = 0) in vec3 pos;

// ----- Definitions ----- //

#include ../../../../Engine/Resources/Engine/Shaders/include/defines.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/structs.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/uniforms.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/functions.glsl

out vec3 localPos;

void main()
{
    localPos = pos;
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0); 
}

#frag 

#version 330 

// ----- Definitions ----- //

#include ../../../../Engine/Resources/Engine/Shaders/include/defines.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/structs.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/uniforms.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/functions.glsl

in vec3 localPos;

out vec4 FragColor;

uniform samplerCube cubemap;

vec3 cartesianToPolar(vec2 uv)
{
    uv /= 2.0; // [-.5, .5]
    uv += 0.5; // [0, 1]
    
    float phi = uv.x * 2.0 * PI; // phi [0, 2 * PI]
    float theta = uv.y * PI; // theta [0, PI]

    // extract x y z from theta and phi
    // x = sin(theta) * cos(phi)
    // y = sin(theta) * sin(phi)
    // z = cos(theta)
    vec3 v = vec3(sin(theta) * cos(phi), cos(theta), sin(theta) * sin(phi));
    return v;
}

void main()
{
    vec3 sample = cartesianToPolar(localPos.xy);
    vec3 color = texture(cubemap, sample).rgb;
    FragColor = vec4(color, 1.0);
}