#pragma once

#include <string>
#include <vector>

#include "Core.h"
#include "VertexArrayObject.h"

class EngineAPI Terrain
{
public:
	static Terrain generateTerrain(int rez, int scale, const std::string& heightMapFilepath);

	std::shared_ptr<VertexArrayObject> getVAO() const;
	int getRez() const;

	Terrain() = default;
private:
	std::shared_ptr<VertexArrayObject> m_vao;
	std::shared_ptr<VertexBufferObject> m_vbo;

	int m_rez = 0;
};