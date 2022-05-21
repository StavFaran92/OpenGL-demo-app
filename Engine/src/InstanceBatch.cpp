#include "InstanceBatch.h"

#include "Transformation.h"

InstanceBatch::InstanceBatch(ObjectHandler<Model> modelHandler, std::vector<Transformation> transformations)
	: m_modelHandler(modelHandler), m_amount(transformations.size())
{
	m_transformationMatrices = std::shared_ptr<glm::mat4[]>(new glm::mat4[m_amount]);
	//m_transformationMatrices = std::make_shared<glm::mat4[]>(m_amount);

	for (int i=0; i< m_amount; i++)
	{
		transformations[i].getMatrix(m_transformationMatrices[i]);
	}
} 

InstanceBatch::InstanceBatch(const InstanceBatch& other)
	: m_modelHandler(other.m_modelHandler), 
	m_amount(other.m_amount), 
	m_transformationMatrices(other.m_transformationMatrices)
{
}

InstanceBatch& InstanceBatch::operator=(InstanceBatch& other)
{
	m_modelHandler = other.m_modelHandler;
	m_amount = other.m_amount;
	m_transformationMatrices = other.m_transformationMatrices;

	return *this;
}

InstanceBatch::~InstanceBatch()
{}

const glm::mat4* InstanceBatch::getTransformations() const
{
	return m_transformationMatrices.get();
}

ObjectHandler<Model> InstanceBatch::getModelHandler() const
{
	return m_modelHandler;
}

size_t InstanceBatch::getAmount() const
{
	return m_amount;
}