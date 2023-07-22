#include "InstanceBatch.h"

#include "Transformation.h"

InstanceBatch::InstanceBatch(std::vector<Transformation>& transformations)
	: m_amount(transformations.size())
{
	m_transformationMatrices = std::shared_ptr<glm::mat4[]>(new glm::mat4[m_amount]);

	for (int i=0; i< m_amount; i++)
	{
		transformations[i].getWorldTransformation(m_transformationMatrices[i]);
	}
} 

const glm::mat4* InstanceBatch::getMatrices() const
{
	return m_transformationMatrices.get();
}

size_t InstanceBatch::getAmount() const
{
	return m_amount;
}