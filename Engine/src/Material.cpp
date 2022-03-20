#include "Material.h"

#include "Shader.h"
#include "Logger.h"

Material::Material()
	:m_shininess(0)
{
}

Material::Material(float shine)
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