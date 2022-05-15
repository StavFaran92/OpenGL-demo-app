#pragma once
#include "Renderer.h"

#include <vector>

class Model;
class Shader;
class Transformation;

class GpuInstancingRenderer : public Renderer
{
public:
	void render(Model* model, std::vector<Transformation*>*,  Shader* shader = nullptr);
	void draw(const VertexArrayObject& vao, Shader& shader) const override;
private:
	size_t m_amount = 0;
};

