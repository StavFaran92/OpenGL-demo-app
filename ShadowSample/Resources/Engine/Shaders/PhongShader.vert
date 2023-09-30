#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 tex;
layout (location = 3) in vec3 color;

// ----- Definitions ----- //

// ----- Structs ----- //

// ----- Out ----- //

out vec3 Color;
out vec2 texCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosInDirLightSpace;

// ----- Uniforms ----- //

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 lightSpaceMatrix;

layout (std140) uniform Time
{
    float time;
};

// ----- Forward Declerations ----- //

// ----- Methods ----- //

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	Color = color;
	
	texCoord = tex;

	Normal = mat3(transpose(inverse(model))) * norm;

	FragPos = (model * vec4(pos, 1.0)).xyz;
	
	FragPosInDirLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0f);
}