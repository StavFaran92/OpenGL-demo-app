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
	void draw(const VertexArrayObject& vao, Shader& shader) const override;
	virtual void SetMVP(Shader& shader) const;
 	void Clear() const override;

	//Methods
	std::shared_ptr<Shader> GetDefaultShader() const;
	std::shared_ptr<ICamera> GetCamera() const;
protected:
	std::shared_ptr<Shader> m_phongShader = nullptr;
	std::shared_ptr<ICamera> m_camera = nullptr;
	glm::mat4 m_projection;
};
