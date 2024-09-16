#pragma once

#include <string>
#include <vector>

#include "Core.h"
#include "VertexArrayObject.h"
#include "Component.h"

class EngineAPI Terrain : public Component
{
public:
	static Terrain generateTerrain(int width, int height, float scale, const std::string& heightMapFilepath);
	static Terrain generateTerrain(int width, int height, float scale, Resource<Texture> heightMap);


	Terrain() = default;

	Resource<Mesh> getMesh() const;
	Resource<Texture> getHeightmap() const;

	float getScale() const;
	int getWidth() const;
	int getHeight() const;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(m_mesh, m_heightmap, m_width, m_height, m_scale);
	}

	
	Resource<Texture> m_heightmap;

	int m_width = 100;
	int m_height = 100;
	int m_scale = 1;

private:
	Resource<Mesh> m_mesh;
};