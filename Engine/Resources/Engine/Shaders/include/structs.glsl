// ----- Structs ----- //

struct Material 
{ 
	// textures 
	sampler2D texture_diffuse; 
	sampler2D texture_specular; 
	sampler2D texture_normal;
}; 

struct PBR_Material 
{ 
	// textures 
	sampler2D albedoMap; 
	sampler2D normalMap; 
	sampler2D metallicMap;
	sampler2D roughnessMap;
	sampler2D aoMap;
}; 

struct PointLight 
{ 
	vec4 position; 
	vec4 color; 
}; 

struct DirLight 
{ 
	vec4 direction; 
	vec4 color; 
}; 