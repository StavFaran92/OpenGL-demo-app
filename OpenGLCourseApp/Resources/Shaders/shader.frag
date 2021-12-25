#version 330

// In
//---
in vec4 vCol;
in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

// Out
//----
out vec4 colour;

// Data structures
//----------------
struct Material {
	// textures
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_diffuse3;
	sampler2D texture_specular1;
	sampler2D texture_specular2;
    float shininess;
}; 

struct Light {
    vec3 position;
	vec3 color;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 
  
// Uniforms
//---------
// material
uniform Material material;

// light
uniform Light light;

// camera
uniform vec3 viewPos;



void main()
{
	// Ambient light
	vec3 ambientColor = (light.ambient *  vec3(texture(material.texture_diffuse1, texCoord))) * light.color;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);

	// Diffuse light
	float diff = max(0.f, dot(lightDir, norm));
	vec3 diffuseColor = ((light.diffuse * vec3(texture(material.texture_diffuse1, texCoord))) * diff) * light.color;

	// Specular light
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectLightDir = reflect(-lightDir, Normal);

	float spec = pow(max(0.f, dot(reflectLightDir, viewDir)), material.shininess);
	vec3 specularLight = ((light.specular * vec3(texture(material.texture_specular1, texCoord))) * spec) * light.color;

	vec3 result = (ambientColor + diffuseColor + specularLight);
	colour = vec4(result, 1.f);
}