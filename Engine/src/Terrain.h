#pragma once

#include <string>
#include <vector>

#include "Core.h"
#include "VertexArrayObject.h"
#include "Component.h"

class EngineAPI Terrain : public Component
{
public:
	//static Terrain generateTerrain(int rez, float scale, const std::string& heightMapFilepath);
	static Terrain generateTerrain(int width, int height, float scale, const std::string& heightMapFilepath);

	VertexArrayObject* getVAO() const;
	float getScale() const;
	Resource<Texture> getHeightmap() const;

	int getWidth() const;
	int getHeight() const;

	Terrain() = default;
private:
	std::shared_ptr<VertexArrayObject> m_vao;
	std::shared_ptr<VertexBufferObject> m_vbo;

	Resource<Texture> m_heightmap;

	int m_width = 0;
	int m_height = 0;
	int m_scale = 0;
};