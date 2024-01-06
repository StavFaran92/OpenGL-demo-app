#version 330 

// ----- Definitions ----- //

#include ../../../../Engine/Resources/Engine/Shaders/include/defines.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/structs.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/uniforms.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/functions.glsl

// ----- In ----- //

in VS_OUT {
    vec3 FragPos;
	vec3 Normal;
    vec2 texCoord;
} fs_in;

// ----- Out ----- //

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 normal;
layout (location = 2) out vec4 albedoSpec;

// ----- Uniforms ----- //
uniform Material material; 

// ----- Forward Declerations ----- //

// ----- Methods ----- //
 
void main() 
{ 	
	gPosition = fs_in.FragPos;
	
	normal = normalize(fs_in.Normal);
	
	albedoSpec.rgb = texture(material.texture_diffuse, fs_in.texCoord).rgb;
	
	//albedoSpec.a = texture(material.texture_specular, fs_in.texCoord).r;
} 