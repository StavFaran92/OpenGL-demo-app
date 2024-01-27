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
	sampler2D texture_albedo; 
	sampler2D texture_normal; 
	sampler2D texture_metallic;
	sampler2D texture_roughness;
	sampler2D texture_ao;
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