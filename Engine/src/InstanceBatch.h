#pragma once

#include <memory>
#include "glm/glm.hpp"

#include "Core.h"

#include "Transformation.h"
//#include "Components.h"

class EngineAPI InstanceBatch : public Component
{
public:
	/** Constructor */
	InstanceBatch(std::vector<Transformation>& transformations);

	const glm::mat4* getMatrices() const;
	size_t getAmount() const;
private:
	std::shared_ptr<glm::mat4[]> m_transformationMatrices;
	int m_amount = 0;
};

