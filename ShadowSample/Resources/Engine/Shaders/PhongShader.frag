#version 330                    
                                   
in vec3 Color;                      
in vec2 texCoord;                   
in vec3 Normal;                     
in vec3 FragPos;                    
                                   
out vec4 colour;                    
                                   
// camera                           
uniform vec3 viewPos;               
                                   
struct Material {                   
    // textures                     
    sampler2D texture_diffuse1;     
    sampler2D texture_diffuse2;     
    sampler2D texture_diffuse3;     
    sampler2D texture_specular1;    
    sampler2D texture_specular2;    
    float shininess;                
};                                  
uniform Material material;          
                                                   
struct PointLight {                                 
    vec3 position;                                  
    vec3 color;                                     
    float constant;                                 
    float linear;                                   
    float quadratic;                                
    vec3 ambient;                                   
    vec3 diffuse;                                   
    vec3 specular;                                  
};                                                  
#define NR_POINT_LIGHTS 16                          
uniform PointLight pointLights[NR_POINT_LIGHTS];    
uniform int pointLightCount;                        
                                           
struct DirLight {                           
    vec3 direction;                         
    vec3 color;                             
    vec3 ambient;                           
    vec3 diffuse;                           
    vec3 specular;                          
};                                          
#define NR_DIR_LIGHT 16                     
uniform DirLight dirLight[NR_DIR_LIGHT];    
uniform int dirLightCount;                  
                                                                                                       
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 aTexCoord)          
{                                                                                                       
    vec3 lightDir = normalize(light.position - fragPos);                                                
                                                                                                        
    // diffuse shading                                                                                  
    float diff = max(dot(normal, lightDir), 0.0);                                                       
                                                                                                        
    // specular shading                                                                                 
    vec3 reflectDir = reflect(-lightDir, normal);                                                       
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);                           
                                                                                                        
    // attenuation                                                                                      
    float distance = length(light.position - fragPos);                                                  
    float attenuation = 1.0 / (light.constant + light.linear * distance +                               
        light.quadratic * (distance * distance));                                                       
                                                                                                        
    // combine results                                                                                  
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, aTexCoord));                 
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, aTexCoord));          
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, aTexCoord));       
    ambient *= attenuation;                                                                             
    diffuse *= attenuation;                                                                             
    specular *= attenuation;                                                                            
    return (ambient + diffuse + specular) * light.color;                                                
}                                                                                                       
                                                                                                   
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec2 aTexCoord)                        
{                                                                                                   
    vec3 lightDir = normalize(-light.direction);                                                    
                                                                                                    
    // diffuse shading                                                                              
    float diff = max(dot(normal, lightDir), 0.0);                                                   
                                                                                                    
    // specular shading                                                                             
    vec3 reflectDir = reflect(-lightDir, normal);                                                   
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);                       
                                                                                                    
    // combine results                                                                              
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, aTexCoord));             
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, aTexCoord));      
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, aTexCoord));   
    return (ambient + diffuse + specular) * light.color;                                            
}                                                                                                   
                                
void main()                         
{                                 
    vec3 result;         
  
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