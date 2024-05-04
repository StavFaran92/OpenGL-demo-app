#include "ShapeFactory.h"

#include "Engine.h"
#include "Context.h"
#include "Mesh.h"
#include "Entity.h"
#include "Material.h"
#include "Box.h"
#include "Quad.h"
#include "Sphere.h"
#include "Scene.h"
#include "Transformation.h"
#include "Shader.h"
//#include "RenderableComponent.h"
#include "Component.h"
#include "CacheSystem.h"

Entity ShapeFactory::createEntity(Scene* scene, const std::string& name)
{
	auto entity = scene->createEntity(name);
	entity.addComponent<RenderableComponent>();
	entity.addComponent<MaterialComponent>();
	entity.addComponent<MeshComponent>();
	return entity;
}

Entity ShapeFactory::createBox(Scene* scene)
{
	static int createdBoxCount = 0;
	auto entity = createEntity(scene, "Box_" + std::to_string(createdBoxCount++));
	auto memoryManager = Engine::get()->getMemoryManagementSystem();
	Resource<Mesh> mesh = memoryManager->createOrGetCached<Mesh>("SGE_BOX_MESH", []() {return Box::createMesh(); });
	entity.getComponent<MeshComponent>().mesh = mesh;
	return entity;
}

Entity ShapeFactory::createQuad(Scene* scene)
{
	static int createdQuadCount = 0;
	auto entity = createEntity(scene, "Quad_" + std::to_string(createdQuadCount++));
	auto memoryManager = Engine::get()->getMemoryManagementSystem();
	Resource<Mesh> mesh = memoryManager->createOrGetCached<Mesh>("SGE_QUAD_MESH", []() {return Quad::createMesh(); });
	entity.getComponent<MeshComponent>().mesh = mesh;
	return entity;
}

Entity ShapeFactory::createSphere(Scene* scene)
{
	static int createdSphereCount = 0;
	auto entity = createEntity(scene, "Sphere_" + std::to_string(createdSphereCount++));
	auto memoryManager = Engine::get()->getMemoryManagementSystem();
	Resource<Mesh> mesh = memoryManager->createOrGetCached<Mesh>("SGE_SPHERE_MESH", []() {return Sphere::createMesh(1, 36, 36); });
	entity.getComponent<MeshComponent>().mesh = mesh;
	return entity;
}
