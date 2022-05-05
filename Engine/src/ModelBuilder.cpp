#include "ModelBuilder.h"

#include <memory>

#include "Model.h"
#include "Logger.h"

ObjectHandler<Model> ModelBuilder::build()
{
	auto mesh = m_meshBuilder->build();

	if (!mesh)
	{
		logError("Model builder failed to build model.");

		delete this;

		return {};
	}

	std::shared_ptr<Material> material = std::make_shared<Material>(32.0f);
	m_modelHandler.object()->useMaterial(material);

	m_modelHandler.object()->addMesh(mesh);

	if (m_shader)
		m_modelHandler.object()->attachShader(m_shader);

	auto tempModelHandler = m_modelHandler;

	// We are finished with the builder and it can now be erased.
	delete this;

	return tempModelHandler;
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
	return *m_meshBuilder;
}

