#pragma once

// Includes
//STL
#include <memory>

#include "glm/glm.hpp"
#include "Core.h"
#include <entt/entt.hpp>

class Shader;
class VertexArrayObject;
class Model;
class ICamera;
class Transformation;
class Mesh;
class Entity;
class Scene;
class Context;
struct CameraComponent;

class EngineAPI IRenderer {
public:
	struct Params
	{
		Scene* scene = nullptr;
		Context* context = nullptr;
		IRenderer* renderer = nullptr;
		entt::registry* registry = nullptr;
		CameraComponent* camera = nullptr;
	};

	struct DrawQueueRenderParams : public Params
	{
		Entity* entity = nullptr;
		Mesh* mesh = nullptr;
		Transformation* transform = nullptr;
		Shader* shader = nullptr;
	};
	enum class DrawType {
		Lines,
		Triangles
	};
public:
	IRenderer() {
		SetDrawType(DrawType::Triangles);
	};

	virtual void draw(const VertexArrayObject& vao, Shader& shader) const = 0;
	virtual void clear() const = 0;

	virtual glm::mat4 getProjection() const = 0;
	virtual void render(const DrawQueueRenderParams& renderParams) = 0;

	void SetDrawType(DrawType drawType);

protected:
	uint8_t m_drawType = 0;
};