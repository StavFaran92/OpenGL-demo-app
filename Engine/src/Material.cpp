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
	if (!shader->IsMaterialsEnabled())
	{
		logError("Shader does not support material");
		return;
	}

	shader->SetFloat("material.shininess", m_shininess);
}

Material::~Material()
{
}