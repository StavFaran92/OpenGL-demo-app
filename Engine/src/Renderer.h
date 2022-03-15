#pragma once
#include "IRenderer.h"

// forward declerations
class ICamera;
class SkyboxRenderer;


class Renderer : public IRenderer
{
public:
	// Constructor
	Renderer();

	// Copy Constructor
	Renderer(const Renderer& other);

	//Overrides
	void Draw(const VertexArrayObject& vao, std::shared_ptr<Shader> shader) const override;
	virtual void SetMVP(std::shared_ptr<Shader>& shader) const;
 	void Clear() const override;

	//static const Renderer defaultRenderer;
	//static const SkyboxRenderer skyboxRenderer;

	//Methods
	std::shared_ptr<Shader> GetDefaultShader() const;
	std::shared_ptr<ICamera> GetCamera() const;
protected:
	std::shared_ptr<Shader> m_defaultShader = nullptr;
	std::shared_ptr<ICamera> m_camera = nullptr;
	glm::mat4 m_projection;
};
