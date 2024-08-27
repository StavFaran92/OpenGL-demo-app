#include "Archiver.h"

#include "Scene.h"

Archiver* Archiver::instance = new Archiver();

SerializedEntity Archiver::serializeEntity(Entity e)
{
	SerializedEntity serializedEntity;
	serializedEntity.entity = e.handler();
	serializedEntity.camera = getComponentIfExists<CameraComponent>(e);
	serializedEntity.collisionBox = getComponentIfExists<CollisionBoxComponent>(e);
	serializedEntity.collisionSphere = getComponentIfExists<CollisionSphereComponent>(e);
	serializedEntity.dLight = getComponentIfExists<DirectionalLight>(e);
	serializedEntity.image = getComponentIfExists<ImageComponent>(e);
	serializedEntity.mat = getComponentIfExists<MaterialComponent>(e);
	serializedEntity.mesh = getComponentIfExists<MeshComponent>(e);
	serializedEntity.nsc = getComponentIfExists<NativeScriptComponent>(e);
	serializedEntity.obj = getComponentIfExists<ObjectComponent>(e);
	serializedEntity.pLight = getComponentIfExists<PointLight>(e);
	serializedEntity.renderableComponent = getComponentIfExists<RenderableComponent>(e);
	serializedEntity.rigidBody = getComponentIfExists<RigidBodyComponent>(e);
	serializedEntity.skybox = getComponentIfExists<SkyboxComponent>(e);
	serializedEntity.transform = getComponentIfExists<Transformation>(e);
	serializedEntity.animator = getComponentIfExists<Animator>(e);

	return serializedEntity;
}

void Archiver::deserializeEntity(SerializedEntity serializedEnt, Scene& scene)
{
	auto e = scene.getRegistry().getRegistry().create(serializedEnt.entity);
	auto entityHandler = Entity(e, &scene.getRegistry());
	if (serializedEnt.transform)
	{
		auto& transform = entityHandler.addComponent<Transformation>(serializedEnt.transform.value());
		transform.m_entity.setRegistry(&scene.getRegistry());
		transform.m_root.setRegistry(&scene.getRegistry());
		transform.m_parent.setRegistry(&scene.getRegistry());

		for (auto [_, entity] : serializedEnt.transform.value().getChildren())
		{
			Entity eChild(entity.handler(), &scene.getRegistry());
			transform.addChild(eChild);
		}
	}
	if (serializedEnt.dLight)
	{
		entityHandler.addComponent<DirectionalLight>(serializedEnt.dLight.value());
	}

	if (serializedEnt.pLight)
	{
		entityHandler.addComponent<PointLight>(serializedEnt.pLight.value());
	}

	if (serializedEnt.mesh)
	{
		entityHandler.addComponent<MeshComponent>(serializedEnt.mesh.value());
	}

	if (serializedEnt.renderableComponent)
	{
		entityHandler.addComponent<RenderableComponent>(serializedEnt.renderableComponent.value());
	}

	if (serializedEnt.camera)
	{
		entityHandler.addComponent<CameraComponent>(serializedEnt.camera.value());

		scene.setPrimaryCamera(entityHandler);
	}

	if (serializedEnt.mat)
	{
		entityHandler.addComponent<MaterialComponent>(serializedEnt.mat.value());
	}

	if (serializedEnt.obj)
	{
		auto& obj = entityHandler.addComponent<ObjectComponent>(serializedEnt.obj.value());
		obj.e.setRegistry(&scene.getRegistry());
	}

	if (serializedEnt.skybox)
	{
		auto skybox = Skybox::CreateSkyboxFromEquirectangularMap(serializedEnt.skybox->originalImage, entityHandler, &scene);
		skybox.getComponent<SkyboxComponent>().originalImage = serializedEnt.skybox->originalImage;
	}

	if (serializedEnt.collisionSphere)
	{
		entityHandler.addComponent<CollisionSphereComponent>(serializedEnt.collisionSphere.value());
	}

	if (serializedEnt.collisionBox)
	{
		entityHandler.addComponent<CollisionBoxComponent>(serializedEnt.collisionBox.value());
	}

	if (serializedEnt.rigidBody)
	{
		entityHandler.addComponent<RigidBodyComponent>(serializedEnt.rigidBody.value());
	}

	if (serializedEnt.image)
	{
		entityHandler.addComponent<ImageComponent>(serializedEnt.image.value());
	}

	if (serializedEnt.nsc)
	{
		auto& nsc = entityHandler.addComponent<NativeScriptComponent>(serializedEnt.nsc.value());
		nsc.entity.setRegistry(&scene.getRegistry());
		nsc.script->entity.setRegistry(&scene.getRegistry());
	}

	if (serializedEnt.animator)
	{
		entityHandler.addComponent<Animator>(serializedEnt.animator.value());
	}
}

SerializedScene Archiver::serializeScene(Scene* scene)
{
	SerializedScene serializedScene;

	scene->getRegistry().get().each([&](auto entity) {
		Entity e(entity, &scene->getRegistry());

		// We only serialize entities that have object component
		//if (e.HasComponent<ObjectComponent>())
		{
			SerializedEntity serializedEntity = serializeEntity(e);

			serializedScene.serializedEntities.push_back(serializedEntity);
		}
		});

	return serializedScene;
}

void Archiver::deserializeScene(SerializedScene serializedScene, Scene& scene)
{
	//scene.getRegistry().getRegistry().clear();

	for (auto& serializedEnt : serializedScene.serializedEntities)
	{
		deserializeEntity(serializedEnt, scene);
	}
}

SerializedContext Archiver::serializeContext(const Context* ctx)
{
	SerializedContext serializedContext;

	for (auto& [sceneID, scene] : ctx->getAllScenes())
	{
		serializedContext.serializedScenes[sceneID] = serializeScene(scene.get());
	}

	serializedContext.activeScene = ctx->getActiveSceneID();

	return serializedContext;
}

void Archiver::deserializeContext(SerializedContext serializedContext, Context* ctx)
{
	ctx->m_scenes.clear();

	for (auto& [sceneID, serializedScene] : serializedContext.serializedScenes)
	{
		std::shared_ptr<Scene> scene = std::make_shared<Scene>(ctx);
		deserializeScene(serializedScene, *scene.get());
		ctx->m_scenes[sceneID] = scene;
	}

	ctx->m_activeScene = serializedContext.activeScene;
}