#pragma once

#include "glm/glm.hpp"
#include <vector>
#include <memory>
#include <type_traits>

#include "Core.h"

#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "MeshBuilder.h"
#include "Texture.h"


class Model;

template<typename T>
class ModelBuilder
{
	static_assert(std::is_base_of<Model, T>::value, "T must inherit from Model");
public:
	/** Constructor */
	template<typename... _Types>
	ModelBuilder(_Types&&... _Args);

	template<typename... _Types>
	static ModelBuilder<T> builder(_Types&&... _Args);
	
	/** Destructor */
	~ModelBuilder() {};

	ModelBuilder<T>& setShader(Shader& shader, bool copy = false);

	MeshBuilder& getMeshBuilder();

	T* build();
private:
	std::shared_ptr<Shader> m_shader = nullptr;
	std::shared_ptr<MeshBuilder> m_meshBuilder = nullptr;
	std::shared_ptr<Model> m_model = nullptr;

	bool isBuilt = false;

};

template<typename T>
T* ModelBuilder<T>::build()
{
	auto mesh = m_meshBuilder->build();

	//TODO optimize: can load textuer on startup and simply assign texture Ptr / ID
	auto texturediff = Texture::loadTextureFromFile("Resources\\Textures\\template.png");
	texturediff->setType(Texture::Type::Diffuse);

	auto textureSpec = Texture::loadTextureFromFile("Resources\\Textures\\template.png");
	textureSpec->setType(Texture::Type::Specular);

	mesh->addTexture(texturediff);
	mesh->addTexture(textureSpec);

	std::shared_ptr<Material> material = std::make_shared<Material>(32.0f);
	m_model->UseMaterial(material);

	m_model->addMesh(mesh);

	return m_model.get();
}

template<class T>
template<typename... _Types>
ModelBuilder<T>::ModelBuilder(_Types&&... _Args)
{
	m_model = std::make_shared<T>(std::forward<_Types>(_Args)...);

	//m_meshBuilder = T::createMeshBuilder(*this);
}


template<class T>
ModelBuilder<T>& ModelBuilder<T>::setShader(Shader& shader, bool copy)
{
	if (copy)
	{
		m_shader = std::make_shared<Shader>(shader);
	}
	else
	{
		m_shader = std::shared_ptr<Shader>(&shader);
	}

	return *this;
}

template<typename T>
template<typename... _Types>
ModelBuilder<T> ModelBuilder<T>::builder(_Types&&... _Args)
{
	return ModelBuilder<T>(std::forward<_Types>(_Args)...);
}


template<class T>
MeshBuilder& ModelBuilder<T>::getMeshBuilder()
{
	return *m_meshBuilder.get();
}
