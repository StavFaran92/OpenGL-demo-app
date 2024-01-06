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

uniform vec3 viewPos; 
uniform Material material; 

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

// ----- Forward Declerations ----- //

// ----- Methods ----- //
 
void main() 
{ 
    // retrieve data from G-buffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    
    // then calculate lighting as usual
    vec3 lighting = Albedo * 0.1; // hard-coded ambient component
    vec3 viewDir = normalize(viewPos - FragPos);
    for(int i = 0; i < NR_POINT_LIGHTS; ++i)
    {
        // diffuse
        vec3 lightDir = normalize(pointLights[i].position.rgb - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * pointLights[i].color.rgb;
        lighting += diffuse;
    }
    
    FragColor = vec4(lighting, 1.0);
} 