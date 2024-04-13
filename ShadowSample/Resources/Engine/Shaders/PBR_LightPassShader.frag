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

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gMRA;
uniform samplerCube gIrradianceMap;
uniform samplerCube gPrefilterEnvMap;
uniform sampler2D gBRDFIntegrationLUT;

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

struct Surface
{
	vec3 fragPos;
	vec3 V;
	vec3 N;
	vec3 H;
	vec3 L;
	vec3 F0;
	vec3 albedo;
	float metallic;
	float roughness;
	
};

vec3 calculateBRDF(Surface s)
{
	// Calculate BRDF
	// Calculate Fresnel Schlick 
	vec3 F = fresnelSchlick(max(0.0, dot(s.H, s.V)), s.F0);

	vec3 ks = F;
	vec3 kd = 1.0 - ks;
	kd *= 1.0 - s.metallic;

	// Calculate NDF
	float NDF = distributionGGX(s.N, s.H, s.roughness);

	// Calculate Schlick-GGX
	float G = geometrySmith(s.N, s.V, s.L, s.roughness);

	vec3 numerator = NDF * G * F;
	float denominator = 4.0 * max(0.0, dot(s.N, s.V)) * max(0.0, dot(s.N, s.L)) + 0.0001;
	vec3 specular = numerator / denominator;

	return (specular + kd * s.albedo / PI);
}

vec3 PointLightRadiance(PointLight pLight, Surface s)
{
	s.L = normalize(pLight.position.rgb - s.fragPos);
	s.H = normalize(s.V + s.L);

	// Calculate Li
	float distance = length(s.L);
	float attenuation = 1.0 / (distance * distance);
	vec3 radiance = pLight.color.rgb * attenuation;

	// Calculate cosTheta
	float cosTheta = max(0.0, dot(s.N, s.L));

	return calculateBRDF(s) * radiance * cosTheta;
}

vec3 DirLightRadiance(DirLight dLight, Surface s)
{
	s.L = dLight.direction.xyz;
	s.H = normalize(s.V + s.L);

	// Calculate Li
	vec3 radiance = dLight.color.rgb;

	// Calculate cosTheta
	float cosTheta = max(0.0, dot(s.N, s.L));

	return calculateBRDF(s) * radiance * cosTheta;
}

uniform vec3 cameraPos;

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

	Surface s;
	s.fragPos = fragPos;
	s.V = V;
	s.N = N;
	s.F0 = F0;
	s.metallic = metallic;
	s.roughness = roughness;
	s.albedo = albedo;

	for(int i = 0; i < pointLightCount; ++i)
	{
		L0 += PointLightRadiance(pointLights[i], s);
	}

	for(int i = 0; i < dirLightCount; ++i)
	{
		L0 += DirLightRadiance(dirLight[i], s);
	}

	// generate Kd to accomodate only for diffuse (exclude specular)
	vec3 F = fresnelSchlickRoughness(max(0.0, dot(N, V)), F0, roughness);
	
	vec3 irradiance = texture(gIrradianceMap, N).rgb;
	vec3 diffuse = irradiance * albedo;

	float shadow = shadowCalculations(vec4(fragPos, 1.f));
	//return (ambient + (1.0 - shadow) * (diffuse + specular)) * light.color.rgb; 

	const float MAX_REFLECTION_LOD = 4.0;
	vec3 prefilterColor = textureLod(gPrefilterEnvMap, R, roughness * MAX_REFLECTION_LOD).rgb;

	vec2 envBRDF = texture(gBRDFIntegrationLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
	vec3 specular = prefilterColor * (envBRDF.x * F + envBRDF.y); 

	vec3 ks = F;
	vec3 kd = 1.0 - ks;

	// ambient diffuse irradiance
	vec3 ambient = (kd * diffuse + specular) * ao * (1.0 - shadow) * vec3(1.f);

	// combine results
	vec3 color = L0 + ambient;

	// HDR
	color = color / (color + vec3(1.0));

	// Gamma correction
	color = pow(color, vec3(1.0/2.2));
    
    FragColor = vec4(color, 1.0);
} 