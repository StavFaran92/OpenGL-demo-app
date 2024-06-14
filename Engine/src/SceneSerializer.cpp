#include "SceneSerializer.h"

#include "Skybox.h"
#include "EquirectangularToCubemapConverter.h"

SerializedScene SceneSerializer::serializeScene(Scene& scene)
{
	// Serialize registry
	SerializedScene serializedScene;

	scene.getRegistry().get().each([&](auto entity) {
		Entity e(entity, &scene.getRegistry());
		
		SerializableEntity se{ entity, 
			getComponentIfExists<Transformation>(e),
			getComponentIfExists<RigidBodyComponent>(e),
			getComponentIfExists<CollisionBoxComponent>(e),
			getComponentIfExists<CollisionSphereComponent>(e),
			getComponentIfExists<MeshComponent>(e),
			getComponentIfExists<RenderableComponent>(e),
			getComponentIfExists<CameraComponent>(e),
			getComponentIfExists<NativeScriptComponent>(e),
			getComponentIfExists<MaterialComponent>(e),
			getComponentIfExists<DirectionalLight>(e),
			getComponentIfExists<PointLight>(e),
			getComponentIfExists<ObjectComponent>(e),
			getComponentIfExists<SkyboxComponent>(e),
		};
		serializedScene.serializedEntities.push_back(se);
	});

	return serializedScene;
}

void SceneSerializer::deserializeScene(const SerializedScene& serializedScene, Scene& scene)
{
	//scene.clear();


	for (auto& serializedEnt : serializedScene.serializedEntities)
	{
		auto e = scene.getRegistry().get().create(serializedEnt.entity);
		auto entityHandler = Entity(e, &scene.getRegistry());
		if (serializedEnt.transform)
		{
			auto& transform = entityHandler.addComponent<Transformation>(serializedEnt.transform.value());
			transform.m_entity.setRegistry(&scene.getRegistry());
			transform.m_root.setRegistry(&scene.getRegistry());
			transform.m_parent.setRegistry(&scene.getRegistry());
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
			Skybox::CreateSkybox(serializedEnt.skybox->skyboxImage, entityHandler, &scene);
		}

		if (serializedEnt.nsc)
		{
			auto& nsc = entityHandler.addComponent<NativeScriptComponent>(serializedEnt.nsc.value());
			nsc.entity.setRegistry(&scene.getRegistry());
			nsc.script->entity.setRegistry(&scene.getRegistry());
			nsc.script->onCreate(); // todo move to scene start play
		}
	}
}
