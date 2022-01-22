#pragma once
//#include "Graphics/Buffers/VertexArrayObject.h"
//#include "Graphics/Shader/Shader.h"
//#include "Graphics/Camera/ICamera.h"
//#include "Graphics/Camera/EditorCamera.h"

#include "IRenderer.h"

// forward declerations
class ICamera;



class Renderer : public IRenderer
{
public:
	Renderer();

	//Overrides
	void Draw(const VertexArrayObject& vao, std::shared_ptr<Shader> shader) const override;
	virtual void SetMVP(std::shared_ptr<Shader>& shader) const;
 	void Clear() const override;

	//Methods
	std::shared_ptr<Shader> GetDefaultShader() const;
	std::shared_ptr<ICamera> GetCamera() const;
protected:
	std::shared_ptr<Shader> m_defaultShader = nullptr;
	std::shared_ptr<ICamera> m_camera = nullptr;
	glm::mat4 m_projection;
};
