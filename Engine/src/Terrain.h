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

	Terrain() = default;

	Resource<Mesh> getMesh() const;
	Resource<Texture> getHeightmap() const;

	float getScale() const;
	int getWidth() const;
	int getHeight() const;

private:
	Resource<Mesh> m_mesh;
	Resource<Texture> m_heightmap;

	int m_width = 0;
	int m_height = 0;
	int m_scale = 0;
};