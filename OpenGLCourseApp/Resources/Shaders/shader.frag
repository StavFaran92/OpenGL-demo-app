#version 330

in vec4 vCol;
in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 colour;


uniform vec3 lightColor;
uniform vec3 lightPos;

uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

void main()
{
	// Ambient light
	float ambientIntensity = 0.1f;
	vec3 ambientColor = lightColor * ambientIntensity;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	// Diffuse light
	float diff = max(0.f, dot(lightDir, norm));
	vec3 diffuseColor = diff * lightColor;

	// Specular light
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectLightDir = reflect(-lightDir, Normal);

	float spec = pow(max(0.f, dot(reflectLightDir, viewDir)), 32);
	vec3 specularLight = lightColor * spec * specularStrength;

	vec4 ccc = texture(texture_specular1, texCoord) ;
	vec3 result = (ambientColor + diffuseColor + specularLight);
	colour = texture(texture_diffuse1, texCoord) * vec4(result, 1.f);
}