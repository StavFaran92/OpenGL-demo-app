#vert

#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 tex;
layout (location = 3) in vec3 color;
layout (location = 4) in vec3 aTangent;
layout (location = 5) in vec3 aBitangent;  

// ----- Definitions ----- //

#include ../../../../Engine/Resources/Engine/Shaders/include/defines.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/structs.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/uniforms.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/functions.glsl

// ----- Structs ----- //

// ----- Out ----- //

out VS_OUT {
	vec3 Color;
    vec3 FragPos;
	vec3 Normal;
    vec2 texCoord;
    vec4 FragPosInDirLightSpace;
    vec3 TangentDirLightPos[NR_DIR_LIGHT];
    vec3 TangentViewPos[NR_POINT_LIGHTS];
    vec3 TangentFragPos;
} vs_out;

// ----- Uniforms ----- //

// ----- Forward Declerations ----- //

// ----- Methods ----- //

void main()
{
	vec3 T = normalize(vec3(model * vec4(aTangent, 0.f)));
	vec3 B = normalize(vec3(model * vec4(aBitangent, 0.f)));
	vec3 N = normalize(vec3(model * vec4(norm, 0.f)));
	mat3 TBN = transpose(mat3(T, B, N));

	vs_out.Color = color;
	vs_out.texCoord = tex;
	vs_out.Normal = mat3(transpose(inverse(model))) * norm;
	vs_out.FragPos = (model * vec4(pos, 1.0)).xyz;
	vs_out.FragPosInDirLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0f);

	gl_Position = projection * view * model * vec4(pos, 1.0);
}

#frag

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

out vec4 colour; 

// ----- Uniforms ----- //

uniform Material material; 

// ----- Forward Declerations ----- //

// ----- Methods ----- //
 
void main() 
{ 
	vec3 result; 
	
	vec3 norm;
	if(true)
	{
		norm = texture(material.texture_normal, fs_in.texCoord).rgb;
		norm = normalize(norm * 2.0 - 1.0); 
 
	}
	else
	{
		norm = normalize(fs_in.Normal);
	}
	vec3 viewPos = vec3(view[0][3], view[1][3], view[2][3]);
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