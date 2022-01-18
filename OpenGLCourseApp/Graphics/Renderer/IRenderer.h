#pragma once

// Includes
//STL
#include <memory>

#include "Graphics/Buffers/VertexArrayObject.h"
#include "glm/glm.hpp"

class Shader;
class VertexArrayObject;

class IRenderer {

public:
	IRenderer() = default;
	virtual void Draw(const VertexArrayObject& vao, std::shared_ptr<Shader> shader) const = 0;
	virtual void Clear() const = 0;
};