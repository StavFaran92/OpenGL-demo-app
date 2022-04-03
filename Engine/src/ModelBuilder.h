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


class Model;

template<class T> 
class EngineAPI ModelBuilder
{
	static_assert(std::is_base_of<Model, T>::value, "T must inherit from Model");
public:
	/** Constructor */
	template<class T>
	ModelBuilder()
	{
		std::cout << "" << std::endl;
		//m_meshBuilder = T::createMeshBuilder(*this);
	}

	/** Destructor */
	inline ~ModelBuilder() {};

	template<class T>
	inline ModelBuilder<T>& setShader(Shader& shader, bool copy = false)
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


	inline MeshBuilder& getMeshBuilder();

	template<class T, class... _Types>
	inline T* build(_Types&&... _Args)
	{
		auto model = std::make_shared<T>(std::forward<_Types>(_Args)...);

		auto mesh = m_meshBuilder->build();

		//TODO optimize: can load textuer on startup and simply assign texture Ptr / ID
		auto texturediff = Texture::loadTextureFromFile("Resources\\Textures\\template.png");
		texturediff->setType(Texture::Type::Diffuse);

		auto textureSpec = Texture::loadTextureFromFile("Resources\\Textures\\template.png");
		textureSpec->setType(Texture::Type::Specular);

		mesh->addTexture(texturediff);
		mesh->addTexture(textureSpec);

		std::shared_ptr<Material> material = std::make_shared<Material>(32.0f);
		model->UseMaterial(material);

		model->addMesh(mesh);

		return model.get();
	}
private:
	std::shared_ptr<Shader> m_shader = nullptr;
	std::shared_ptr<MeshBuilder> m_meshBuilder = nullptr;

	bool isBuilt = false;

};

//template<class T>
//ModelBuilder<T>::ModelBuilder()
//{
//	m_meshBuilder = T::createMeshBuilder(*this);
//}


//template<class T>
//ModelBuilder<T>& ModelBuilder<T>::setShader(Shader& shader, bool copy)
//{
//	if (copy)
//	{
//		m_shader = std::make_shared<Shader>(shader);
//	}
//	else
//	{
//		m_shader = std::shared_ptr<Shader>(&shader);
//	}
//
//	return *this;
//}


template<class T>
MeshBuilder& ModelBuilder<T>::getMeshBuilder()
{
	return *m_meshBuilder.get();
}
