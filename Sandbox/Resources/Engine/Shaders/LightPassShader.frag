#version 330 

// ----- Definitions ----- //

#include ../../../../Engine/Resources/Engine/Shaders/include/defines.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/structs.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/uniforms.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/functions.glsl

// ----- In ----- //

in vec2 TexCoords;

// ----- Out ----- //

out vec4 FragColor;

// ----- Uniforms ----- //
 
uniform Material material; 

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gSSAOColorBuffer;

// ----- Forward Declerations ----- //

// ----- Methods ----- //
 
void main() 
{ 
    // retrieve data from G-buffer
    vec3 fragPos = texture(gPosition, TexCoords).rgb;
    vec3 normal = texture(gNormal, TexCoords).rgb;
    vec3 albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float gSpecular = texture(gAlbedoSpec, TexCoords).a;
    float ambientOcclusion = texture(gSSAOColorBuffer, TexCoords).r;
    
    // then calculate lighting as usual
    vec3 lighting = vec3(0.3 * albedo * ambientOcclusion);;
    vec3 viewDir = normalize(-fragPos);
    for(int i = 0; i < pointLightCount; ++i)
    {	
		vec3 lightDir = normalize(pointLights[i].position.rgb - fragPos); 
		
		// diffuse shading 
		float diff = max(dot(normal, lightDir), 0.0); 
		
		// specular shading 
		vec3 reflectDir = reflect(-lightDir, normal); 
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f); 
		
		// attenuation 
		float distance = length(pointLights[i].position.rgb - fragPos); 
		float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance)); 
		
		// combine results 
		vec3 diffuse = lightDiffuse * diff * albedo;
		vec3 specular = lightSpecular * spec * gSpecular * vec3(1.0f);
		diffuse *= attenuation; 
		specular *= attenuation; 
		lighting += (diffuse + specular) * pointLights[i].color.rgb;
    }
    
    FragColor = vec4(lighting, 1.0);
} 