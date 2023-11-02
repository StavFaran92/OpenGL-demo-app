#pragma once
#include "IRenderer.h"

#include "Core.h"

// forward declerations
class ICamera;
class SkyboxRenderer;
class Model;
class Mesh;
class Transformation;
class Shader;
class Entity;
class Mesh;


class EngineAPI Renderer : public IRenderer
{
public:
	// Constructor
	Renderer();

	//Overrides
	void draw(const VertexArrayObject& vao) const override;
 	void clear() const override;
	void render(const DrawQueueRenderParams& renderParams);

	void enableWireframeMode(bool enable);
	// Inherited via IRenderer
	void renderScene(DrawQueueRenderParams& renderParams) override;

private:
	void setUniforms(const DrawQueueRenderParams& renderParams);
protected:
	glm::mat4 m_projection;

	bool m_wireFrameMode = false;





};
