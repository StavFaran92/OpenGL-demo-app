#include "ShapeFactory.h"

#include "Engine.h"
#include "Context.h"
#include "Mesh.h"
#include "Entity.h"
#include "DefaultMaterial.h"
#include "Box.h"
#include "Quad.h"
#include "Sphere.h"
#include "Scene.h"
#include "Transformation.h"
#include "Shader.h"
//#include "RenderableComponent.h"
#include "Component.h"
#include "MemoryManagement.h"

Entity ShapeFactory::createEntity(Scene* scene)
{
	auto entity = scene->createEntity();
	entity.addComponent<DefaultMaterial>(32.0f);
	auto shader = Shader::create<Shader>();
	entity.addComponent<Shader>(shader);
	entity.addComponent<RenderableComponent>();
	return entity;
}

Entity ShapeFactory::createBox(Scene* scene)
{
	auto entity = createEntity(scene);
	auto memoryManager = Engine::get()->getMemoryManagementSystem();
	std::shared_ptr<Mesh> mesh = memoryManager->getMesh("SGE_BOX_MESH", []() {return Box::createMesh(); });
	entity.addComponent<MeshComponent>(mesh);
	return entity;
}

Entity ShapeFactory::createQuad(Scene* scene)
{
	auto entity = createEntity(scene);
	auto memoryManager = Engine::get()->getMemoryManagementSystem();
	std::shared_ptr<Mesh> mesh = memoryManager->getMesh("SGE_QUAD_MESH", []() {return Quad::createMesh(); });
	entity.addComponent<MeshComponent>(mesh);
	return entity;
}

Entity ShapeFactory::createSphere(Scene* scene)
{
	auto entity = createEntity(scene);
	auto memoryManager = Engine::get()->getMemoryManagementSystem();
	std::shared_ptr<Mesh> mesh = memoryManager->getMesh("SGE_SPHERE_MESH", []() {return Sphere::createMesh(1, 36, 36); });
	entity.addComponent<MeshComponent>(mesh);
	return entity;
}
