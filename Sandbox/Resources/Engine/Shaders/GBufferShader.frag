#version 330 

// ----- Definitions ----- //

#include ../../../../Engine/Resources/Engine/Shaders/include/defines.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/structs.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/uniforms.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/functions.glsl

// ----- In ----- //

in VS_OUT {
	vec3 Color;
    vec3 FragPos;
	vec3 Normal;
    vec2 texCoord;
    vec4 FragPosInDirLightSpace;
    vec3 TangentDirLightPos[NR_DIR_LIGHT];
    vec3 TangentViewPos[NR_POINT_LIGHTS];
    vec3 TangentFragPos;
} fs_in;

// ----- Out ----- //

layout (location = 0) out vec3 position;
layout (location = 1) out vec3 normal;
layout (location = 2) out vec4 albedoSpec;

// ----- Uniforms ----- //
uniform Material material; 

// ----- Forward Declerations ----- //

// ----- Methods ----- //
 
void main() 
{ 	
	position = fs_in.FragPos;
	
	normal = normalize(fs_in.Normal);
	
	albedoSpec.rgb = texture(material.texture_diffuse, fs_in.texCoord).rgb;
	
	//albedoSpec.a = texture(material.texture_specular, fs_in.texCoord).r;
} 