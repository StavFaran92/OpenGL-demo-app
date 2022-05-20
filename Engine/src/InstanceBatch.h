#pragma once

#include <memory>
#include "glm/glm.hpp"

#include "Core.h"

#include "ObjectHandler.h"

class Model;
class Transformation;

class EngineAPI InstanceBatch
{
public:
	/** Constructor */
	InstanceBatch(ObjectHandler<Model> modelHandler, std::vector<Transformation> transformations);

	/** Copy Constructor */
	InstanceBatch(const InstanceBatch& other);

	/** Move Constructor */
	InstanceBatch& operator=(InstanceBatch& other);

	const glm::mat4* getTransformations() const;
	ObjectHandler<Model> getModelHandler() const;
	size_t getAmount() const;

	/** Destructor */
	~InstanceBatch();
private:
	ObjectHandler<Model> m_modelHandler;
	std::shared_ptr<glm::mat4[]> m_transformationMatrices = nullptr;
	int m_amount = 0;
};

