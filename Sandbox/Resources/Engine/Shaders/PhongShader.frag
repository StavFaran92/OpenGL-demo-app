#version 330 

// ----- Definitions ----- //

// ----- In ----- //

in vec3 Color; 
in vec2 texCoord; 
in vec3 Normal; 
in vec3 FragPos; 
in vec4 FragPosInDirLightSpace;

// ----- Out ----- //

out vec4 colour; 

// ----- Includes ----- //

#include ../../../../Engine/Resources/Engine/Shaders/include/defines.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/structs.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/uniforms.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/functions.glsl

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
		norm = texture(material.texture_normal, texCoord).rgb;
		norm = normalize(norm * 2.0 - 1.0); 
 
	}
	else
	{
		norm = normalize(Normal);
	}
	vec3 viewDir = normalize(viewPos - FragPos); 
	
	// Directional lighting 
	for (int i = 0; i < dirLightCount; i++) 
	{
		result += CalcDirLight(dirLight[i], norm, viewDir, texCoord, material); 
	}
	
	for (int i = 0; i < pointLightCount; i++) 
	{
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, texCoord, material); 
	}
 
	colour = vec4(result,1.0); 
} 