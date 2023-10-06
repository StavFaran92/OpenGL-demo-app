// ----- Uniforms buffers ----- //

layout (std140) uniform Lights
{
	int pointLightCount;
	int dirLightCount;
	PointLight pointLights[NR_POINT_LIGHTS];
	DirLight dirLight[NR_DIR_LIGHT]; 
};

// ----- Uniforms ----- //

uniform sampler2D shadowMap;