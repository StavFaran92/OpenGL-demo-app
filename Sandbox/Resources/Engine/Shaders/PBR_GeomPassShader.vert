#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 tex;

// ----- Definitions ----- //

#include ../../../../Engine/Resources/Engine/Shaders/include/defines.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/structs.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/uniforms.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/functions.glsl

// ----- Structs ----- //

// ----- Out ----- //

out VS_OUT {
    vec3 fragPos;
	vec3 normal;
    vec2 texCoord;
} vs_out;

// ----- Uniforms ----- //

// ----- Forward Declerations ----- //

// ----- Methods ----- //

void main()
{
	mat4 modelViewMat = view * model;
	vs_out.texCoord = tex;
	vs_out.normal = mat3(transpose(inverse(model))) * norm;
	vs_out.fragPos = (model * vec4(pos, 1.0)).xyz;

	gl_Position = projection * modelViewMat * vec4(pos, 1.0);
}