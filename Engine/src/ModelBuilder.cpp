#include "ModelBuilder.h"

#include <memory>

#include "Model.h"

Model* ModelBuilder::build()
{
	auto mesh = m_meshBuilder->build();

	//TODO optimize: can load textuer on startup and simply assign texture Ptr / ID
	auto texturediff = Texture::loadTextureFromFile("Resources\\Textures\\template.png");
	texturediff->setType(Texture::Type::Diffuse);

	auto textureSpec = Texture::loadTextureFromFile("Resources\\Textures\\template.png");
	textureSpec->setType(Texture::Type::Specular);

	mesh->addTexture(std::shared_ptr<Texture>(texturediff));
	mesh->addTexture(std::shared_ptr<Texture>(textureSpec));

	std::shared_ptr<Material> material = std::make_shared<Material>(32.0f);
	m_model->UseMaterial(material);

	m_model->addMesh(mesh);

	if (m_shader)
		m_model->AttachShader(m_shader);

	auto tempModel = m_model;

	// We are finished with the builder and it can now be erased.
	delete this;

	return tempModel;
}

ModelBuilder& ModelBuilder::setShader(Shader& shader, bool copy)
{
	if (copy)
	{
		m_shader = std::make_shared<Shader>(shader);
	}
	else
	{
		m_shader = std::shared_ptr<Shader>(&shader);
	}

	return *this;
}

MeshBuilder& ModelBuilder::getMeshBuilder()
{
	return *m_meshBuilder.get();
}

