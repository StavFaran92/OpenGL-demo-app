#pragma once
#include "Renderer/Buffers/VertexArrayObject.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Camera/ICamera.h"
#include "Renderer/Camera/EditorCamera.h"

#include "glm/glm.hpp"

class Renderer
{
public:
	Renderer();
	void Draw(const VertexArrayObject& vao, std::shared_ptr<Shader> shader) const;
	std::shared_ptr<Shader> GetDefaultShader() const;
	std::shared_ptr<ICamera> GetCamera();
 	void Clear() const;
private:
	std::shared_ptr<Shader> m_defaultShader = nullptr;
	std::shared_ptr<ICamera> m_camera = nullptr;
	glm::mat4 m_projection;
};
