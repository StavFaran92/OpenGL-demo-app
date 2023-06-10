#pragma once

#include "IRenderer.h"

class Model;
class ICamera;

class Renderer2D : public IRenderer
{
public:
	Renderer2D() = default;
	// Inherited via IRenderer
	void draw(const VertexArrayObject& vao, Shader& shader) const override;
	void clear() const override;

	// Inherited via IRenderer
	glm::mat4 getProjection() const override;

	// Inherited via IRenderer
	//virtual void render(Model* model, Shader* shader = nullptr) override;

	// Inherited via IRenderer
	virtual std::shared_ptr<ICamera> getCamera() const override;

	// Inherited via IRenderer
	virtual void render(Entity* entity, Mesh* mesh, Transformation* transform, Shader* shader = nullptr) override;
};

