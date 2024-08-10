#vert

#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 tex;
layout (location = 5) in ivec3 boneIDs;
layout (location = 6) in vec3 boneWeights;

layout (location = 7) in mat4 instanceModel;

// ----- Definitions ----- //

#include ../../../../Engine/Resources/Engine/Shaders/include/defines.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/structs.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/uniforms.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/functions.glsl

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 3;

// ----- Structs ----- //

// ----- Out ----- //

out VS_OUT {
    vec3 fragPos;
	vec3 normal;
    vec2 texCoord;
} vs_out;

// ----- Uniforms ----- //

uniform mat4 finalBonesMatrices[MAX_BONES];

// ----- Forward Declerations ----- //

// ----- Methods ----- //

float getTime()
{
	return time;
}

#ifdef CUSTOM_SHADER
#custom_vert
#endif

void main()
{
	mat4 aModel = model;

	if(isGpuInstanced)
	{
		aModel = model * instanceModel;
	}

	vec4 totalPosition = vec4(0.0f);
	bool useBones = false;
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(boneIDs[i] == -1) 
            continue;
        if(boneIDs[i] >=MAX_BONES) 
        {
            totalPosition = vec4(pos,1.0f);
            break;
        }

		useBones = true;

        vec4 localPosition = finalBonesMatrices[boneIDs[i]] * vec4(pos,1.0f);
        totalPosition += localPosition * boneWeights[i];
        // vec3 localNormal = mat3(finalBonesMatrices[boneIDs[i]]) * norm; // todo fix
    }

	if(!useBones)
	{
		totalPosition = vec4(pos, 1.0f);
	}

	vec3 aNorm = mat3(transpose(inverse(aModel))) * norm;
#ifdef CUSTOM_SHADER
	vert(totalPosition.xyz, aNorm);
#endif

	vs_out.texCoord = tex;
	vs_out.normal =  aNorm;
	vs_out.fragPos = (aModel * totalPosition).xyz;

	gl_Position = projection * view * aModel * totalPosition;
}

#frag

#version 330 

// ----- Definitions ----- //

#include ../../../../Engine/Resources/Engine/Shaders/include/defines.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/structs.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/uniforms.glsl
#include ../../../../Engine/Resources/Engine/Shaders/include/functions.glsl

// ----- In ----- //

in VS_OUT {
    vec3 fragPos;
	vec3 normal;
    vec2 texCoord;
} fs_in;

// ----- Out ----- //

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec3 gMRA;

// ----- Uniforms ----- //
uniform PBR_Material material; 

// ----- Forward Declerations ----- //

// ----- Methods ----- //
 
void main() 
{ 	
	gPosition = fs_in.fragPos;
	gNormal = normalize(fs_in.normal) * texture(material.texture_normal, fs_in.texCoord).rgb;
	gAlbedo = texture(material.texture_albedo, fs_in.texCoord).rgb;
	gMRA.r = texture(material.texture_metallic, fs_in.texCoord).r;
	gMRA.g = texture(material.texture_roughness, fs_in.texCoord).r;
	gMRA.b = texture(material.texture_ao, fs_in.texCoord).r;
} 