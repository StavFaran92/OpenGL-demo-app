#include "SceneSerializer.h"

#include "Scene.h"
#include "cereal/archives/json.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/optional.hpp"
#include "cereal/types/polymorphic.hpp"
#include <optional>
#include <fstream>
#include <filesystem>
#include <ostream>

#include "Component.h"
#include "Transformation.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "PointLight.h"

struct SerializableEntity
{
	entt::entity entity;
	std::optional<Transformation> transform;
	std::optional<RigidBodyComponent> rigidBody;
	std::optional<CollisionBoxComponent> collisionBox;
	std::optional<CollisionSphereComponent> collisionSphere;
	std::optional<MeshComponent> mesh;
	std::optional <RenderableComponent> renderableComponent;
	std::optional<CameraComponent> camera;
	//std::optional<NativeScriptComponent> nsc;
	std::optional<Material> mat;
	std::optional<DirectionalLight> dLight;
	std::optional<PointLight> pLight;
	std::optional<ObjectComponent> obj;
	std::optional<SkyboxComponent> skybox;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(entity, transform, rigidBody, collisionBox, collisionSphere, mesh, renderableComponent, camera,
			mat, dLight, pLight, obj, skybox);
	}
};

namespace glm
{
	template<class Archive>
	void serialize(Archive& archive, glm::vec3& v) {
		archive(v.x, v.y, v.z);
	}

	template<class Archive>
	void serialize(Archive& archive, glm::quat& q) {
		archive(q.x, q.y, q.z, q.w);
	}
};

template<typename T>
decltype(auto) getComponentIfExists(const Entity& e)
{
	std::optional<T> c;
	if (e.HasComponent<T>())
	{
		c = e.getComponent<T>();
	}
	return c;
}

bool SceneSerializer::serialize(Scene& scene)
{
	std::vector<SerializableEntity> serializedEntities;

	scene.getRegistry().each([&](auto entity) {
		Entity e(entity, &scene);
		
		SerializableEntity se{ entity, 
			getComponentIfExists<Transformation>(e),
			getComponentIfExists<RigidBodyComponent>(e),
			getComponentIfExists<CollisionBoxComponent>(e),
			getComponentIfExists<CollisionSphereComponent>(e),
			getComponentIfExists<MeshComponent>(e),
			getComponentIfExists<RenderableComponent>(e),
			getComponentIfExists<CameraComponent>(e),
			//getComponentIfExists<NativeScriptComponent>(e),
			getComponentIfExists<Material>(e),
			getComponentIfExists<DirectionalLight>(e),
			getComponentIfExists<PointLight>(e),
			getComponentIfExists<ObjectComponent>(e),
			getComponentIfExists<SkyboxComponent>(e),
		};
		serializedEntities.push_back(se);
	});

	//if(!std::filesystem::exists("entities.json"))

	std::ofstream os("entities.json");
	cereal::JSONOutputArchive archive(os);
	archive(serializedEntities);

	return true;
}

bool SceneSerializer::deserialize(Scene& scene)
{
	scene.clear();

	std::vector<SerializableEntity> serializedEntities;

	std::ifstream is("entities.json");
	cereal::JSONInputArchive inputArchive(is);
	inputArchive(serializedEntities);

	for (auto& serializedEnt : serializedEntities)
	{
		auto e = scene.getRegistry().create(serializedEnt.entity);
		auto entityHandler = Entity(e, &scene);
		if (serializedEnt.transform)
		{
			entityHandler.addComponent<Transformation>(serializedEnt.transform.value());
		}

	}

	return true;
}
