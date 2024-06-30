#pragma once

#include "Scene.h"
#include "cereal/archives/json.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/types/optional.hpp"
#include "cereal/types/unordered_map.hpp"
#include <optional>
#include <fstream>
#include <filesystem>
#include <ostream>

#include "Component.h"
#include "Transformation.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Mesh.h"
#include "Context.h"
#include "ProjectAssetRegistry.h"
#include "EditorCamera.h"


#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>
#include <assimp/types.h>

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
decltype(auto) getComponentIfExists(const Entity& e)
{
	std::optional<T> c;
	if (e.HasComponent<T>())
	{
		c = e.getComponent<T>();
	}
	return c;
}

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
	std::optional<NativeScriptComponent> nsc;
	std::optional<MaterialComponent> mat;
	std::optional<DirectionalLight> dLight;
	std::optional<PointLight> pLight;
	std::optional<ObjectComponent> obj;
	std::optional<SkyboxComponent> skybox;
	std::optional<ImageComponent> image;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(entity, transform, rigidBody, collisionBox, collisionSphere, mesh, renderableComponent, camera,
			nsc, mat, dLight, pLight, obj, skybox, image);
	}
};

struct SerializedScene
{
	std::vector<SerializableEntity> serializedEntities;

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

class SceneSerializer
{
public:
	static SerializedScene serializeScene(Scene& scene);
	static void deserializeScene(const SerializedScene& serializedScene, Scene& scene);
};