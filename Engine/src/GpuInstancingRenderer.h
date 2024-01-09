#pragma once
#include "Renderer.h"

#include <vector>
#include "VertexArrayObject.h"

class Model;
class Shader;
class Transformation;
class InstanceBatch;

class GpuInstancingRenderer : public Renderer
{
public:
	GpuInstancingRenderer(std::shared_ptr<FrameBufferObject> renderTarget, Scene* scene);
	void render(const DrawQueueRenderParams& renderParams);
	void draw(const VertexArrayObject& vao) const override;
private:
	size_t m_amount = 0;
	std::shared_ptr<VertexBufferObject> m_vbo;
	unsigned int buffer;

};

