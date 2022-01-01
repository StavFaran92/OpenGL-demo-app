#pragma once
#include "Renderer/Buffers/VertexArrayObjectWrapper.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Camera/ICamera.h"
#include "Renderer/Camera/FlyCamera.h"

#include "glm/glm.hpp"

class Renderer
{
public:
	Renderer();
	void Draw(const VertexArrayObjectWrapper& vao, std::shared_ptr<Shader> shader) const;
	std::shared_ptr<Shader> GetDefaultShader() const;
	std::shared_ptr<ICamera> GetCamera();
 	void Clear() const;
private:
	std::shared_ptr<Shader> m_defaultShader = nullptr;
	std::shared_ptr<ICamera> m_camera = nullptr;
	glm::mat4 m_projection;
};
