#include "ShapeFactory.h"

#include "Engine.h"
#include "Context.h"
#include "Mesh.h"
#include "Entity.h"
#include "Material.h"
#include "Box.h"
#include "Quad.h"
#include "Sphere.h"

std::shared_ptr<Entity> ShapeFactory::createEntity()
{
	auto scene = Engine::get()->getContext()->getActiveScene();
	auto entity = scene->createEntity();
	entity->addComponent<Material>(32.0f);
	return entity;
}

std::shared_ptr<Entity> ShapeFactory::createBox()
{
	auto entity = createEntity();
	auto mesh = Box::createMesh();
	entity->addComponent<Mesh>(mesh);
	return entity;
}

std::shared_ptr<Entity> ShapeFactory::createPlane()
{
	auto entity = createEntity();
	auto mesh = Quad::createMesh();
	entity->addComponent<Mesh>(mesh);
	return entity;
}

std::shared_ptr<Entity> ShapeFactory::createSphere(float radius, int sectors, int stacks)
{
	auto entity = createEntity();
	auto mesh = Sphere::createMesh(radius, sectors, stacks);
	entity->addComponent<Mesh>(mesh);
	return entity;
}
