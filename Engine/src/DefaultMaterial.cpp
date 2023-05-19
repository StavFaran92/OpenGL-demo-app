#include "DefaultMaterial.h"

DefaultMaterial::DefaultMaterial() : Material()
{
}

DefaultMaterial::DefaultMaterial(float shine) : Material(shine)
{
}

void DefaultMaterial::UseMaterial(Shader& shader)
{
	Material::UseMaterial(shader);

	shader.setFloat("material.shininess", m_shininess);
}

DefaultMaterial::~DefaultMaterial()
{
}
