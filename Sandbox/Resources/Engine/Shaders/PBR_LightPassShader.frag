#version 330 

// ----- Definitions ----- //

#include ../../../../Engine/Resources/Engine/Shaders/include/defines.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/structs.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/uniforms.glsl

// ----- In ----- //

in vec2 TexCoords;

// ----- Out ----- //

out vec4 FragColor;

// ----- Uniforms ----- //

uniform vec3 cameraPos;
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gMRA;
uniform samplerCube gIrradianceMap;
uniform samplerCube gPrefilterEnvMap;
uniform sampler2D gBRDFIntegrationLUT;
uniform sampler2D gShadowMap;

// ----- Forward Declerations ----- //

// ----- Methods ----- //
 
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}   

float geometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float k)
{
	float NdotV = max(0.0, dot(N, V));
	float NdotL = max(0.0, dot(N, L));
	return geometrySchlickGGX(NdotV, k) * geometrySchlickGGX(NdotL, k);
}

float distributionGGX(vec3 N, vec3 H, float a)
{
	float a2 = a * a;
	float NdotH = max(0.0, dot(N, H));
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = NdotH2 * (a2 - 1.0) + 1.0;
	denom = denom * denom * PI;

	return nom / denom;
}

float shadowCalculations(vec4 fragPos)
{
	// perform perspective divide
    vec3 projCoords = fragPos.xyz / fragPos.w;
	
	vec2 texelSize = 1.0 / textureSize(gShadowMap, 0);
	
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
			float pcfDepth = texture(gShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += (currentDepth - bias > pcfDepth) ? 1.0 : 0.0;
		}
	}
	
	shadow /= 9.0;
	
	return shadow;
}

void main() 
{ 
    // retrieve data from G-buffer
    vec3 fragPos = texture(gPosition, TexCoords).rgb;
    vec3 normal = texture(gNormal, TexCoords).rgb;
    vec3 albedo = pow(texture(gAlbedo, TexCoords).rgb, vec3(2.2));
	float metallic = texture(gMRA, TexCoords).r;
	float roughness = texture(gMRA, TexCoords).g;
	float ao = texture(gMRA, TexCoords).b;

	vec3 F0 = vec3(0.04); // every dieltctric object has F0 = 0.04
	F0 = mix(F0, albedo, metallic);

	// Reflectance equation
	// L0(P, W0) = integral[ BRDF(P, W0, Wi, roughness) * Li(P, Wi) * cosTheta(n, Wi) * dw ]
	// BRDF = [ DFG / (4 * dot(n, w0) * dot(n, wi)) ] + Kd * albedo / PI
	vec3 N = normalize(normal);
	vec3 V = normalize(cameraPos - fragPos);
	vec3 R = reflect(-V, N);
    vec3 L0 = vec3(0.0);
    for(int i = 0; i < pointLightCount; ++i)
    {
		vec3 L = normalize(pointLights[i].position.rgb - fragPos);
		vec3 H = normalize(V + L);

		// Calculate Li
		float distance = length(L);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = pointLights[i].color.rgb * attenuation;

		// Calculate cosTheta
		float cosTheta = max(0.0, dot(N, L));

		// Calculate BRDF
		// Calculate Fresnel Schlick 
		vec3 F = fresnelSchlick(max(0.0, dot(H, V)), F0);

		vec3 ks = F;
		vec3 kd = 1.0 - ks;
		kd *= 1.0 - metallic;

		// Calculate NDF
		float NDF = distributionGGX(N, H, roughness);

		// Calculate Schlick-GGX
		float G = geometrySmith(N, V, L, roughness);

		vec3 numerator = NDF * G * F;
		float denominator = 4.0 * max(0.0, dot(N, V)) * max(0.0, dot(N, L)) + 0.0001;
		vec3 specular = numerator / denominator;

		L0 += (specular + kd * albedo / PI) * radiance * cosTheta;
    }

	// generate Kd to accomodate only for diffuse (exclude specular)
	vec3 F = fresnelSchlickRoughness(max(0.0, dot(N, V)), F0, roughness);
	
	vec3 irradiance = texture(gIrradianceMap, N).rgb;
	vec3 diffuse = irradiance * albedo;

	const float MAX_REFLECTION_LOD = 4.0;
	vec3 prefilterColor = textureLod(gPrefilterEnvMap, R, roughness * MAX_REFLECTION_LOD).rgb;

	vec2 envBRDF = texture(gBRDFIntegrationLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
	vec3 specular = prefilterColor * (envBRDF.x * F + envBRDF.y); 

	vec3 ks = F;
	vec3 kd = 1.0 - ks;

	// ambient diffuse irradiance
	vec3 ambient = (kd * diffuse + specular) * ao;

	// combine results
	vec3 color = L0 + ambient;

	// HDR
	color = color / (color + vec3(1.0));

	// Gamma correction
	color = pow(color, vec3(1.0/2.2));
    
    FragColor = vec4(color, 1.0);
} 