#pragma once

// Includes
//STL
#include <memory>

#include "VertexArrayObject.h"
#include "glm/glm.hpp"
#include "Logger.h"

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
	virtual void Draw(const VertexArrayObject& vao, std::shared_ptr<Shader> shader) const = 0;
	virtual void Clear() const = 0;
	void SetDrawType(DrawType drawType)
	{
		switch (drawType)
		{
		case DrawType::Lines:
			m_drawType = GL_LINES;
			break;
		case DrawType::Triangles:
			m_drawType = GL_TRIANGLES;
			break;
		default:
			logError("Illegal draw type specified.");
		}
	}
protected:
	uint8_t m_drawType = 0;
};