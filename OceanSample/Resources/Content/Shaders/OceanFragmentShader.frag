#version 330                    

// ----- Definitions ----- //

#define NR_POINT_LIGHTS 8 
#define NR_DIR_LIGHT 8 

// ----- In ----- //

in vec3 Color;                      
in vec2 texCoord;                   
in vec3 Normal;                     
in vec3 FragPos;                    

// ----- Out ----- //

out vec4 colour;                    
                                   
// ----- Structs ----- //

struct Material 
{                                      
    sampler2D texture_diffuse;          
    sampler2D texture_specular;      
    float shininess;                
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

// ----- Uniforms ----- //

uniform vec3 viewPos;                                          
uniform Material material;          
             
layout (std140) uniform Lights
{
	int pointLightCount;
	int dirLightCount;
	PointLight pointLights[NR_POINT_LIGHTS];
	DirLight dirLight[NR_DIR_LIGHT]; 
};  

// ----- Forward Declerations ----- //

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 aTexCoord);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec2 aTexCoord);

// ----- Methods ----- //
                                                                                                       
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 aTexCoord) 
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
 
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec2 aTexCoord) 
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
	
	return (ambient + diffuse + specular) * light.color.rgb; 
}                                                                                       
                                
void main()                         
{                                 
    vec3 result = vec3(0.0f, 0.329f, 0.576f);         
  
	// properties                                                                                        
	vec3 norm = normalize(Normal);                                                                       
	vec3 viewDir = normalize(viewPos - FragPos);                                                         

	// Directional lighting                                                                              
	for (int i = 0; i < dirLightCount; i++)                                                              
		result += CalcDirLight(dirLight[i], norm, viewDir, texCoord);                                    

	// Point lights                                                                                      
	for (int i = 0; i < pointLightCount; i++)                                                            
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, texCoord);                      
 
    colour = vec4(result,1.0);              
}          