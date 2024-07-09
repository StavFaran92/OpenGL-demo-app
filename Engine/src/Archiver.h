#pragma once

#include <cereal/types/polymorphic.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/optional.hpp>
#include <cereal/archives/json.hpp>
#include <iostream>
#include <fstream>

#include "Component.h"
#include "Transformation.h"
#include "EditorCamera.h"
#include "ScriptableEntity.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Scene.h"
#include "Skybox.h"
#include "Context.h"
#include "Core.h"

namespace glm
{
	template<class Archive>
	void serialize(Archive& archive, glm::vec3& v) {
		archive(v.x, v.y, v.z);
	}

	template<class Archive>
	void serialize(Archive& archive, glm::vec2& v) {
		archive(v.x, v.y);
	}

	template<class Archive>
	void serialize(Archive& archive, glm::quat& q) {
		archive(q.x, q.y, q.z, q.w);
	}
};

template<typename T>
std::optional<T> getComponentIfExists(const Entity& e)
{
	std::optional<T> c;
	if (e.HasComponent<T>())
	{
		c = e.getComponent<T>();
	}
	return c;
}

struct SerializedEntity
{
	entt::entity entity;
	std::optional<Transformation> transform;
	std::optional<RigidBodyComponent> rigidBody;
	std::optional<CollisionBoxComponent> collisionBox;
	std::optional<CollisionSphereComponent> collisionSphere;
	std::optional<MeshComponent> mesh;
	std::optional<RenderableComponent> renderableComponent;
	std::optional<CameraComponent> camera;
	std::optional<NativeScriptComponent> nsc;
	std::optional<MaterialComponent> mat;
	std::optional<DirectionalLight> dLight;
	std::optional<PointLight> pLight;
	std::optional<ObjectComponent> obj;
	std::optional<SkyboxComponent> skybox;
	std::optional<ImageComponent> image;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(entity,
			transform,
			rigidBody,
			collisionBox,
			collisionSphere,
			mesh,
			renderableComponent,
			camera,
			nsc,
			mat,
			dLight,
			pLight,
			obj,
			skybox,
			image);
	}
};

struct SerializedScene
{
	std::vector<SerializedEntity> serializedEntities;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(serializedEntities);
	}
};

struct SerializedContext
{
	std::map<int, SerializedScene> serializedScenes;
	int activeScene = 0;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(serializedScenes, activeScene);
	}
};

class EngineAPI Archiver
{
public:
	static void save()
	{
		//SerializedContext serializedContext = Archiver::serializeContext(Engine::get()->getContext());

		//auto projectDir = Engine::get()->getProjectDirectory();
		//std::ofstream os(projectDir + "/entities.json");
		//cereal::JSONOutputArchive oarchive(os);
		//oarchive(serializedContext);

		//Engine::get()->getContext()->save();

		instance->m_serializeCallback();

		logInfo("Successfully Saved Project.");
	}

	static void load()
	{
		instance->m_deserializeCallback();

		logInfo("Successfully Loaded Project.");
	}

	static void registerSerializeFunction(std::function<void()> cb)
	{
		instance->m_serializeCallback = cb;
	}

	static void registerDeserializeFunction(std::function<void()> cb)
	{
		instance->m_deserializeCallback = cb;
	}

	static SerializedEntity serializeEntity(Entity e)
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

		return serializedEntity;
	}

	static void deserializeEntity(SerializedEntity serializedEnt, Scene& scene)
	{
		auto e = scene.getRegistry().getRegistry().create(serializedEnt.entity);
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
	}

	static SerializedScene serializeScene(Scene* scene)
	{
		SerializedScene serializedScene;

		scene->getRegistry().get().each([&](auto entity) {
			Entity e(entity, &scene->getRegistry());

			SerializedEntity serializedEntity = serializeEntity(e);

			serializedScene.serializedEntities.push_back(serializedEntity);
			});

		return serializedScene;
	}

	static void deserializeScene(SerializedScene serializedScene, Scene& scene)
	{
		for (auto& serializedEnt : serializedScene.serializedEntities)
		{
			deserializeEntity(serializedEnt, scene);
		}
	}

	static SerializedContext serializeContext(const Context* ctx)
	{
		SerializedContext serializedContext;

		for (auto& [sceneID, scene] : ctx->getAllScenes())
		{
			serializedContext.serializedScenes[sceneID] = serializeScene(scene.get());
		}

		serializedContext.activeScene = ctx->getActiveSceneID();

		return serializedContext;
	}

	static void deserializeContext(SerializedContext serializedContext, Context* ctx)
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

	std::function<void()> m_serializeCallback;
	std::function<void()> m_deserializeCallback;

	static Archiver* instance;
};