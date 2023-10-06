// ----- forward decleration ----- //

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 aTexCoord, Material material);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec2 aTexCoord, Material material);
float shadowCalculations(vec4 fragPos);

// ----- functions ----- //

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 aTexCoord, Material material) 
{ 
	float constant = 1.f;
	float linear = .35f;
	float quadratic = .44f;

	float lightAmbient = .2f;
	float lightDiffuse = .5f;
	float lightSpecular = 1.f;
	
	vec3 lightDir = normalize(light.position.rgb - fragPos); 
	
	// diffuse shading 
	float diff = max(dot(normal, lightDir), 0.0); 
	
	// specular shading 
	vec3 reflectDir = reflect(-lightDir, normal); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); 
	
	// attenuation 
	float distance = length(light.position.rgb - fragPos); 
	float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance)); 
	
	// combine results 
	vec3 ambient = lightAmbient * vec3(texture(material.texture_diffuse, aTexCoord)); 
	vec3 diffuse = lightDiffuse * diff * vec3(texture(material.texture_diffuse, aTexCoord)); 
	vec3 specular = lightSpecular * spec * vec3(texture(material.texture_specular, aTexCoord)); 
	ambient *= attenuation; 
	diffuse *= attenuation; 
	specular *= attenuation; 
	return (ambient + diffuse + specular) * light.color.rgb; 
} 
 
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec2 aTexCoord, Material material) 
{ 
	float lightAmbient = .2f;
	float lightDiffuse = .5f;
	float lightSpecular = 1.f;
	
	vec3 lightDir = normalize(-light.direction.rgb); 
	
	// diffuse shading 
	float diff = max(dot(normal, lightDir), 0.0); 
	
	// specular shading 
	vec3 reflectDir = reflect(-lightDir, normal); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); 
	
	// combine results 
	vec3 ambient = lightAmbient * vec3(texture(material.texture_diffuse, aTexCoord)); 
	vec3 diffuse = lightDiffuse * diff * vec3(texture(material.texture_diffuse, aTexCoord)); 
	vec3 specular = lightSpecular * spec * vec3(texture(material.texture_specular, aTexCoord)); 
	
	float shadow = shadowCalculations(FragPosInDirLightSpace);
	return (ambient + (1.0 - shadow) * (diffuse + specular)) * light.color.rgb; 
}

float shadowCalculations(vec4 fragPos)
{
	// perform perspective divide
    vec3 projCoords = fragPos.xyz / fragPos.w;
	
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	
	projCoords = projCoords * 0.5 + 0.5; 
	
	float borderBias =  max(texelSize.x, texelSize.y) * 2;
	
	if(projCoords.x >= 1.0 - borderBias || projCoords.x <= borderBias ||
		projCoords.y >= 1.0 - borderBias || projCoords.y <= borderBias ||
		projCoords.z >= 1.0 - borderBias || projCoords.z <= borderBias)
        return 0.0;
	
	float shadow = 0;
	float bias = 0.005;
	float currentDepth = projCoords.z;
	
	
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += (currentDepth - bias > pcfDepth) ? 1.0 : 0.0;
		}
	}
	
	shadow /= 9.0;
	
	return shadow;
}