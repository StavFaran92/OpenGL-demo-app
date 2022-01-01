#include "Material.h"

Material::Material()
	:m_shininess(0)
{
}

Material::Material(GLfloat shine)
: m_shininess(shine)
{
}

void Material::UseMaterial(std::shared_ptr<Shader> shader)
{
	shader->SetFloat("material.shininess", m_shininess);
}

Material::~Material()
{
}