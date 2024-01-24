#version 330 

// ----- Definitions ----- //

#include ../../../../Engine/Resources/Engine/Shaders/include/defines.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/structs.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/uniforms.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/functions.glsl

// ----- In ----- //

in VS_OUT {
    vec3 fragPos;
	vec3 normal;
    vec2 texCoord;
} fs_in;

// ----- Out ----- //

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec3 gMRA;

// ----- Uniforms ----- //
uniform PBR_Material material; 

// ----- Forward Declerations ----- //

// ----- Methods ----- //
 
void main() 
{ 	
	gPosition = fs_in.fragPos;
	
	gNormal = fs_in.normal * texture(material.normalMap, fs_in.texCoord).rgb;
	gAlbedo = texture(material.albedoMap, fs_in.texCoord).rgb;
	gMRA.r = texture(material.metallicMap, fs_in.texCoord).r;
	gMRA.g = texture(material.roughnessMap, fs_in.texCoord).r;
	gMRA.b = texture(material.aoMap, fs_in.texCoord).r;
} 