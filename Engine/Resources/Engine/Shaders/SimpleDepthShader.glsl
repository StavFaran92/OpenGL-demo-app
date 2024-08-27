#vert

#version 330 core
layout (location = 0) in vec3 pos;

layout (location = 5) in ivec3 boneIDs;
layout (location = 6) in vec3 boneWeights;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 3;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;
uniform bool isAnimated;

uniform mat4 finalBonesMatrices[MAX_BONES];

void main()
{
    vec4 totalPosition = vec4(pos, 1.0f);

    if(isAnimated)
	{
		totalPosition = vec4(0.0f);

		for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
		{
			if(boneIDs[i] == -1) 
				continue;
			if(boneIDs[i] >=MAX_BONES) 
			{
				totalPosition = vec4(pos,1.0f);
				break;
			}

			vec4 localPosition = finalBonesMatrices[boneIDs[i]] * vec4(pos,1.0f);
			totalPosition += localPosition * boneWeights[i];
		}
	}

    gl_Position = lightSpaceMatrix * model * totalPosition;
}  

#frag

#version 330 core

void main()
{             
    // gl_FragDepth = gl_FragCoord.z;
}  