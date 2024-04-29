#include "SceneSerDes.h"

#include "Scene.h"
#include "cereal/archives/json.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/optional.hpp"
#include <optional>
#include <fstream>
#include <filesystem>
#include <ostream>

#include "Component.h"
#include "Transformation.h"

struct SerializableEntity
{
	entt::entity entity;
	std::optional<Transformation> transform;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(entity, transform);
	}
};

bool SceneSerDes::serialize(Scene& scene)
{
	std::vector<SerializableEntity> serializedEntities;

	auto& registry = scene.getRegistry();

	registry.each([&](auto entity) {
		std::optional<Transformation> t;
		if (registry.any_of<Transformation>(entity)) {
			t = *registry.try_get<Transformation>(entity);
		}
		SerializableEntity e{ entity, t };
		serializedEntities.push_back(e);
		});

	//if(!std::filesystem::exists("entities.json"))

	std::ofstream os("entities.json");
	cereal::JSONOutputArchive archive(os);
	archive(serializedEntities);

	return true;
}

bool SceneSerDes::deserialize(Scene& scene)
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
