#pragma once

// Includes
//STL
#include <memory>

#include "glm/glm.hpp"
#include "Core.h"

class Shader;
class VertexArrayObject;

class EngineAPI IRenderer {
public:
	enum class DrawType {
		Lines,
		Triangles
	};
public:
	IRenderer() {
		SetDrawType(DrawType::Triangles);
	};
	virtual void draw(const VertexArrayObject& vao, Shader& shader) const = 0;
	virtual void Clear() const = 0;
	virtual glm::mat4 getProjection() const = 0;
	void SetDrawType(DrawType drawType);

protected:
	uint8_t m_drawType = 0;
};