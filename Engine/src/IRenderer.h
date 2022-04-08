#pragma once

// Includes
//STL
#include <memory>

#include "glm/glm.hpp"


class Shader;
class VertexArrayObject;

class IRenderer {
public:
	enum class DrawType {
		Lines,
		Triangles
	};
public:
	IRenderer() {
		SetDrawType(DrawType::Triangles);
	};
	virtual void Draw(const VertexArrayObject& vao, Shader& shader) const = 0;
	virtual void Clear() const = 0;
	void SetDrawType(DrawType drawType);

protected:
	uint8_t m_drawType = 0;
};