#include "ScreenQuad.h"

#include "Vertex.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Quad.h"

#include "Resources/Primitives/quad.h"

std::shared_ptr<ScreenQuad> ScreenQuad::GenerateScreenQuad()
{
	auto model = std::make_shared<ScreenQuad>();

	auto mesh = Quad::generateMesh();

	model->m_meshes.push_back(std::shared_ptr<Mesh>(mesh));

	return model;
}

void ScreenQuad::Draw(std::shared_ptr<IRenderer> renderer, std::shared_ptr<Shader> shader)
{
	auto currShader = m_shader;

	if (shader)
		currShader = shader;

	renderer->SetDrawType(Renderer::DrawType::Triangles);

	for (auto i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i]->renderMesh(currShader, renderer);
	}
}
