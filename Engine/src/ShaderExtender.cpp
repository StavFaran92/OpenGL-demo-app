#include "ShaderExtender.h"

std::string version = 
"                               \n\
#version 330                    \n\
"; 

std::string inValues = 
"                                   \n\
in vec3 Color;                      \n\
in vec2 texCoord;                   \n\
in vec3 Normal;                     \n\
in vec3 FragPos;                    \n\
";

std::string outValues =
"                                   \n\
out vec4 colour;                    \n\
";

std::string materialStruct =
"                                   \n\
struct Material {                   \n\
    // textures                     \n\
    sampler2D texture_diffuse1;     \n\
    sampler2D texture_diffuse2;     \n\
    sampler2D texture_diffuse3;     \n\
    sampler2D texture_specular1;    \n\
    sampler2D texture_specular2;    \n\
    float shininess;                \n\
};                                  \n\
uniform Material material;          \n\
";

std::string pointLightStruct = 
"                                                   \n\
struct PointLight {                                 \n\
    vec3 position;                                  \n\
    vec3 color;                                     \n\
    float constant;                                 \n\
    float linear;                                   \n\
    float quadratic;                                \n\
    vec3 ambient;                                   \n\
    vec3 diffuse;                                   \n\
    vec3 specular;                                  \n\
};                                                  \n\
#define NR_POINT_LIGHTS 16                          \n\
uniform PointLight pointLights[NR_POINT_LIGHTS];    \n\
uniform int pointLightCount;                        \n\
";
std::string calcPointLight =
"                                                                                                       \n\
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 aTexCoord)          \n\
{                                                                                                       \n\
    vec3 lightDir = normalize(light.position - fragPos);                                                \n\
                                                                                                        \n\
    // diffuse shading                                                                                  \n\
    float diff = max(dot(normal, lightDir), 0.0);                                                       \n\
                                                                                                        \n\
    // specular shading                                                                                 \n\
    vec3 reflectDir = reflect(-lightDir, normal);                                                       \n\
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);                           \n\
                                                                                                        \n\
    // attenuation                                                                                      \n\
    float distance = length(light.position - fragPos);                                                  \n\
    float attenuation = 1.0 / (light.constant + light.linear * distance +                               \n\
        light.quadratic * (distance * distance));                                                       \n\
                                                                                                        \n\
    // combine results                                                                                  \n\
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, aTexCoord));                 \n\
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, aTexCoord));          \n\
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, aTexCoord));       \n\
    ambient *= attenuation;                                                                             \n\
    diffuse *= attenuation;                                                                             \n\
    specular *= attenuation;                                                                            \n\
    return (ambient + diffuse + specular) * light.color;                                                \n\
}                                                                                                       \n\
";

std::string dirLightStruct =
"                                           \n\
struct DirLight {                           \n\
    vec3 direction;                         \n\
    vec3 color;                             \n\
    vec3 ambient;                           \n\
    vec3 diffuse;                           \n\
    vec3 specular;                          \n\
};                                          \n\
#define NR_DIR_LIGHT 16                     \n\
uniform DirLight dirLight[NR_DIR_LIGHT];    \n\
uniform int dirLightCount;                  \n\
";

std::string calcDirLight =
"                                                                                                   \n\
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec2 aTexCoord)                        \n\
{                                                                                                   \n\
    vec3 lightDir = normalize(-light.direction);                                                    \n\
                                                                                                    \n\
    // diffuse shading                                                                              \n\
    float diff = max(dot(normal, lightDir), 0.0);                                                   \n\
                                                                                                    \n\
    // specular shading                                                                             \n\
    vec3 reflectDir = reflect(-lightDir, normal);                                                   \n\
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);                       \n\
                                                                                                    \n\
    // combine results                                                                              \n\
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, aTexCoord));             \n\
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, aTexCoord));      \n\
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, aTexCoord));   \n\
    return (ambient + diffuse + specular) * light.color;                                            \n\
}                                                                                                   \n\
";

std::string lightMainPost = 
"                                                                                                    \n\
// properties                                                                                        \n\
vec3 norm = normalize(Normal);                                                                       \n\
vec3 viewDir = normalize(viewPos - FragPos);                                                         \n\
                                                                                                     \n\
// Directional lighting                                                                              \n\
for (int i = 0; i < dirLightCount; i++)                                                              \n\
    result += CalcDirLight(dirLight[i], norm, viewDir, texCoord);                                    \n\
                                                                                                     \n\
// Point lights                                                                                      \n\
for (int i = 0; i < pointLightCount; i++)                                                            \n\
    result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, texCoord);                      \n\
";

std::string newMainPre =
"                                   \n\
void main()                         \n\
{                                   \n\
";

std::string newMainMid =
"                                   \n\
    vec3 result = smain();         \n\
";

std::string newMainPost =
"                                           \n\
    colour = vec4(result,1.0);              \n\
}                                           \n\
";

std::string ShaderExtender::ExtendFragmentShader(const std::string& code, ExtensionParams eParams)
{
    std::string aditionalCode;

    aditionalCode += version;
    aditionalCode += inValues;
    aditionalCode += outValues;

    if (eParams.enableMaterial)
    {
        aditionalCode += materialStruct;
    }

    if (eParams.enableLight)
    {
        aditionalCode += pointLightStruct;
        aditionalCode += dirLightStruct;
        aditionalCode += calcPointLight;
        aditionalCode += calcDirLight;
    }

    // Set aditional code in shader
    std::string newCode = aditionalCode;

    // Add user code to shader
    newCode += code;

    // replace user main with engine main
    std::string searchString = "main()";
    std::string replaceString = "smain()";

    size_t pos = 0;
    while ((pos = newCode.find(searchString, pos)) != std::string::npos)
    {
        newCode.replace(pos, searchString.length(), replaceString);
        pos += replaceString.length();
    }

    // Append new main that calls smain
    newCode += newMainPre;
    newCode += newMainMid;
    newCode += lightMainPost;
    newCode += newMainPost;

    return newCode;
}
