#include "ModelBuilder.h"

#include <memory>

#include "Model.h"
#include "Logger.h"

Model* ModelBuilder::build()
{
	auto mesh = m_meshBuilder->build();

	if (!mesh)
	{
		logError("Model builder failed to build model.");

		delete this;

		return nullptr;
	}

	std::shared_ptr<Material> material = std::make_shared<Material>(32.0f);
	m_model->useMaterial(material);

	m_model->addMesh(mesh);

	if (m_shader)
		m_model->attachShader(m_shader);

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
	return *m_meshBuilder;
}

