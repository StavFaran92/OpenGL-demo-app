#pragma once
#include "Renderer.h"

#include <vector>
#include "VertexArrayObject.h"

class Model;
class Shader;
class Transformation;

class GpuInstancingRenderer : public Renderer
{
public:
	GpuInstancingRenderer();
	void render(Model* model, std::vector<Transformation*>*,  Shader* shader = nullptr);
	void draw(const VertexArrayObject& vao, Shader& shader) const override;
private:
	size_t m_amount = 0;
	std::shared_ptr<VertexBufferObject> m_vbo;
	unsigned int buffer;

};

