#include "ModelBuilder.h"

#include <memory>

#include "Model.h"
#include "Logger.h"
#include "StandardShader.h"
#include "DefaultMaterial.h"

ObjectHandler<Model> ModelBuilder::build()
{
	auto mesh = m_meshBuilder->build();

	if (!mesh)
	{
		logError("Model builder failed to build model.");

		delete this;

		return {};
	}

	std::shared_ptr<Material> material = std::make_shared<DefaultMaterial>(32.0f);
	m_modelHandler.object()->setMaterial(material);

	m_modelHandler.object()->addMesh(mesh);

	if (m_shader)
		m_modelHandler.object()->attachShader(m_shader);

	if (!m_textureHandlers->empty())
	{
		material->addTextureHandlers(*m_textureHandlers.get());
	}
	else
	{
		//TODO refactor
		//TODO optimize: can load textuer on startup and simply assign texture Ptr / ID
		auto texturediff = Texture::loadTextureFromFile("Resources/Engine/Textures/template.png");
		if (!texturediff)
		{
			logError("Failed to load resource");
			return {};
		}
		texturediff->setType(Texture::Type::Diffuse);

		auto textureSpec = Texture::loadTextureFromFile("Resources/Engine/Textures/template.png");
		if (!textureSpec)
		{
			logError("Failed to load resource");
			return {};
		}
		textureSpec->setType(Texture::Type::Specular);

		material->addTextureHandler(texturediff);
		material->addTextureHandler(textureSpec);
	}

	auto tempModelHandler = m_modelHandler;

	// We are finished with the builder and it can now be erased.
	delete this;

	return tempModelHandler;
}

ModelBuilder::ModelBuilder()
{
	m_textureHandlers = std::make_shared<std::vector<TextureHandler*>>();
}

ModelBuilder& ModelBuilder::setShader(StandardShader* shader, bool copy)
{
	if (copy)
	{
		m_shader = std::make_shared<StandardShader>(*shader);
	}
	else
	{
		m_shader = std::shared_ptr<StandardShader>(shader);
	}

	return *this;
}

MeshBuilder& ModelBuilder::getMeshBuilder()
{
	return *m_meshBuilder;
}

ModelBuilder& ModelBuilder::addTextureHandler(TextureHandler* textureHandler, bool copy)
{
	if (copy)
	{
		throw std::exception("Not implemented yet.");
	}
	else
	{
		m_textureHandlers->push_back(textureHandler);
	}

	return *this;
}

ModelBuilder& ModelBuilder::addTextureHandlers(std::vector<TextureHandler*>& textures, bool copy)
{
	for (auto texture : textures)
	{
		addTextureHandler(texture, copy);
	}

	return *this;
}