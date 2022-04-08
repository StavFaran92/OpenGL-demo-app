#include "ScreenQuad.h"

#include "Vertex.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Quad.h"
#include "ModelBuilder.h"

#include "Resources/Primitives/quad.h"

std::shared_ptr<ScreenQuad> ScreenQuad::GenerateScreenQuad()
{
	return std::shared_ptr<ScreenQuad>((ScreenQuad*)ModelBuilder::builder<ScreenQuad>().build());
}

void ScreenQuad::Draw(std::shared_ptr<IRenderer> renderer, std::shared_ptr<Shader> shader)
{
	auto currShader = m_shader;

	if (shader)
		currShader = shader;

	renderer->SetDrawType(Renderer::DrawType::Triangles);

	for (auto i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i]->render(currShader, renderer);
	}
}