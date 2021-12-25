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
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
// Uniforms
//---------
// material
uniform Material material;

// light
uniform vec3 lightColor;
uniform vec3 lightPos;

// camera
uniform vec3 viewPos;

// textures
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

void main()
{
	// Ambient light
	vec3 ambientColor = lightColor * material.ambient;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	// Diffuse light
	float diff = max(0.f, dot(lightDir, norm));
	vec3 diffuseColor = (diff * material.diffuse) * lightColor;

	// Specular light
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectLightDir = reflect(-lightDir, Normal);

	float spec = pow(max(0.f, dot(reflectLightDir, viewDir)), material.shininess);
	vec3 specularLight = lightColor * spec * material.specular;

	vec4 ccc = texture(texture_specular1, texCoord) ;
	vec3 result = (ambientColor + diffuseColor + specularLight);
	colour = texture(texture_diffuse1, texCoord) * vec4(result, 1.f);
}