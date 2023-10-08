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
	mat3 TBN;
} fs_in;

// ----- Out ----- //

out vec4 colour; 

// ----- Uniforms ----- //

uniform vec3 viewPos; 
uniform Material material; 

// ----- Forward Declerations ----- //

// ----- Methods ----- //
 
void main() 
{ 
	vec3 result; 
	
	vec3 norm;
	if(material.useNormal)
	{
		norm = texture(material.texture_normal, fs_in.texCoord).rgb;
		norm = norm * 2.0 - 1.0; 
		norm = normalize(fs_in.TBN * norm); 
 
	}
	else
	{
		norm = normalize(fs_in.Normal);
	}
	vec3 viewDir = normalize(viewPos - fs_in.FragPos); 
	
	// Directional lighting 
	for (int i = 0; i < dirLightCount; i++) 
	{
		result += CalcDirLight(dirLight[i], norm, viewDir, fs_in.texCoord, material, fs_in.FragPosInDirLightSpace); 
	}
	
	for (int i = 0; i < pointLightCount; i++) 
	{
		result += CalcPointLight(pointLights[i], norm, fs_in.FragPos, viewDir, fs_in.texCoord, material); 
	}
 
	colour = vec4(result,1.0); 
} 