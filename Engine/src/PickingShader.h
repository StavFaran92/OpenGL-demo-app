#pragma once
#include "Shader.h"
class PickingShader : public Shader
{
public:
	PickingShader();

	void setModelMatrix(glm::mat4 model);
	void setViewMatrix(glm::mat4 view);
	void setProjectionMatrix(glm::mat4 projection);

	void setObjectIndex(unsigned int index);
	void setDrawIndex(uint32_t index);
};

