#include "ScreenQuad.h"

#include "Vertex.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Quad.h"
#include "ModelBuilder.h"
#include "ObjectHandler.h"
#include "ShapeFactory.h"
#include "Entity.h"
#include "Scene.h"
#include "Component.h"

#include "RenderableComponent.h"


#include "Resources/Engine/Primitives/quad.h"

std::shared_ptr<Entity> ScreenQuad::GenerateScreenQuad(Scene* scene)
{
	auto entity = ShapeFactory::createPlane(scene);
	entity->RemoveComponent<RenderableComponent>();
	return entity;
}

//void ScreenQuad::draw(IRenderer& renderer, Shader* shader)
//{
//	Shader* currShader = (Shader*)m_shader.get();
//
//	if (shader)
//		currShader = shader;
//
//	renderer.SetDrawType(Renderer::DrawType::Triangles);
//
//	for (auto i = 0; i < m_meshes.size(); i++)
//	{
//		m_meshes[i]->render(*currShader, renderer);
//	}
//}