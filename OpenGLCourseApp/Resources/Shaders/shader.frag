#version 330

in vec4 vCol;
in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 colour;

struct DirectionalLight
{
	vec3 color;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform sampler2D myTexture;
uniform DirectionalLight directionalLight;
uniform Material material;

uniform vec3 eyePosition;

void main()
{
	vec4 ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;

	float diffuseFactor = max(0.0f, dot(normalize(Normal), normalize(directionalLight.direction)));
	vec4 diffuseColor = vec4(directionalLight.color, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

	vec4 specularColor = vec4(0,0,0,0);

	if(diffuseFactor >0.0f)
	{
		vec3 fragToEye = normalize(eyePosition - FragPos);
		vec3 reflectedVertex = normalize(reflect(directionalLight.direction, normalize(Normal)));

		float specularFactor = dot(fragToEye, reflectedVertex);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColor = vec4(directionalLight.color * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	colour = texture(myTexture, texCoord) * (ambientColor + diffuseColor + specularColor);
}