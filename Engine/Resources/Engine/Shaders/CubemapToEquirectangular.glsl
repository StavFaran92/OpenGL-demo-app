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
    gl_Position = projection * view * vec4(pos, 1.0);
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

const vec2 invAtan = vec2(0.1591, 0.3183);
vec3 cartesianToPolar(vec2 uv)
{
    uv -= 0.5; // [-.5, .5]
    uv /= invAtan; // multiply by 2PI and PI to transform to [theta, phi]
    float theta = uv.x;
    float phi = uv.y;

    // extract x y z from theta and phi
    // x = sin(theta) * cos(phi)
    // y = sin(theta) * sin(phi)
    // z = cos(theta)
    vec3 v = vec3(sin(phi) * cos(theta), sin(phi) * sin(theta), cos(phi));
    return v;
}

void main()
{
    vec3 sample = cartesianToPolar(localPos.xy);
    vec3 color = texture(cubemap, sample).rgb;
    FragColor = vec4(color, 1.0);
}