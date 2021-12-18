#include "Material.h"

Material::Material()
	:m_specularIntensity(0), m_shininess(0)
{
}

Material::Material(GLfloat sIntensity, GLfloat shine)
	:m_specularIntensity(sIntensity), m_shininess(shine)
{
}

void Material::UseMaterial(Shader& shader)
{
	shader.SetFloat("material.specularIntensity", m_specularIntensity);
	shader.SetFloat("material.shininess", m_shininess);
}

Material::~Material()
{
}
