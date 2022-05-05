#include "ScreenQuad.h"

#include "Vertex.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Quad.h"
#include "ModelBuilder.h"
#include "ObjectHandler.h"

#include "Resources/Primitives/quad.h"

ObjectHandler<ScreenQuad> ScreenQuad::GenerateScreenQuad()
{
	return ModelBuilder::builder<ScreenQuad>().build();
}

void ScreenQuad::draw(IRenderer& renderer, Shader* shader)
{
	Shader* currShader = m_shader.get();

	if (shader)
		currShader = shader;

	renderer.SetDrawType(Renderer::DrawType::Triangles);

	for (auto i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i]->render(*currShader, renderer);
	}
}