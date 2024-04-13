#version 330 

// ----- Definitions ----- //

#include ../../../../Engine/Resources/Engine/Shaders/include/defines.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/structs.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/uniforms.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/functions.glsl

in vec2 TexCoords;

out vec2 FragColor;

float RadicalInverse_VdC(uint bits) 
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
// ----------------------------------------------------------------------------
vec2 Hammersley(uint i, uint N)
{
    return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}  

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
    float a = roughness * roughness;

    // create angles
    float phi = Xi.x * 2.0 * PI;

    // I dont understand this, but I notice when roughness is larger cosTheta is larger which means 
    // the vector spread is more sparse.
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y)); 

    float sinTheta = sqrt(1.0 - cosTheta * cosTheta); 

    // create H in cartesian
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

    // create TBN
    // I dont understand this ternary and it's reasoning
    vec3 up = (abs(N.z) < .999) ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);

    vec3 tangent = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);

    // return H in world space
    vec3 sampleVector = H.x * tangent + H.y * bitangent + H.z * N;
    return normalize(sampleVector);
}

float geometrySchlickGGX(float NdotV, float roughness)
{
    float a = roughness;
    float k = (a*a) / 2.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float geometrySmith(float NdotV, float NdotL, float k)
{
	return geometrySchlickGGX(NdotV, k) * geometrySchlickGGX(NdotL, k);
}

vec2 integrateBRDF(float NdotV, float roughness)
{
    // create isotropic V 
    vec3 V;
    V.x = sqrt(1.0 - NdotV * NdotV); // sin
    V.y = 0.0;
    V.z = NdotV; // cos

    vec3 N = vec3(0.0, 0.0, 1.0); 

    // loop num of samples
    uint numOfSamples = 1024u;
    float A = 0.0;
    float B = 0.0;
    for(uint i=0u; i < numOfSamples; i++)
    {
        vec2 Xi = Hammersley(i, numOfSamples);
        vec3 H = ImportanceSampleGGX(Xi, N, roughness);
        // vec3 L = 2 * H - V;
        vec3 L  = normalize(2.0 * dot(V, H) * H - V); // I dont understand why use dot here

        float NdotL = L.z;
        float NdotH = H.z;
        float VdotH = max(0.0, dot(V, H));

        if(NdotL > 0.0)
        {
            float G = geometrySmith(NdotV, NdotL, roughness);
            float GVis = (G * VdotH) / (NdotH * NdotV);
            float Fc = pow(1.0 - VdotH, 5.0);

            A += GVis * (1.0 - Fc);
            B += GVis * Fc;
        }
    }

    A /= float(numOfSamples);
    B /= float(numOfSamples);

    return vec2(A, B);
}

void main()
{
    FragColor = integrateBRDF(TexCoords.x, TexCoords.y);
}