#pragma once

#include <vector>
#include <memory>
#include <unordered_set>
#include <unordered_map>

class Model;
class Skybox;
class Context;
class Object3D;
class PointLight;
class DirectionalLight;

class Scene
{

public:
	// -------------------- Methods -------------------- //
	
	void addObject(std::shared_ptr<Object3D> object);
	void removeObject(std::shared_ptr<Object3D> object);

	void setSkybox(std::shared_ptr<Skybox> skybox);
	void removeSkybox();

	std::shared_ptr<Skybox> getSkybox();

private:
	// -------------------- Methods -------------------- //
	friend class Context;
	void update(float deltaTime);
	inline void SetID(uint32_t id) { m_id = id; }
	void draw();

	//template<typename T> std::vector<T*> getAllObjectsFromSet(std::unordered_set<uint32_t> set)
	//{
	//	std::vector<T*> result;
	//	for (auto iter = set.begin(); iter != set.end(); ++iter)
	//	{
	//		result.emplace_back(m_objects.at(*iter));
	//	}
	//	return result;
	//}

	void init();
	void clear();
	void close();

private:
	// -------------------- Attributes -------------------- //
	//std::unordered_map<uint32_t, std::shared_ptr<Object3D>> m_objects;
	std::unordered_map <uint32_t, std::shared_ptr<Model>> m_models;
	std::unordered_map<uint32_t, std::shared_ptr<PointLight>> m_pointLights;
	std::unordered_map<uint32_t, std::shared_ptr<DirectionalLight>> m_directionalLights;

	uint32_t m_id = 0;


	std::shared_ptr<Skybox> m_skybox = nullptr;

};
