#pragma once

#include <vector>

#include "Core.h"
#include "Mesh.h"
#include "Resource.h"

class EngineAPI MeshCollection
{
public:
	void addMesh(const std::shared_ptr<Mesh>& mesh);

	std::shared_ptr<Mesh> getPrimaryMesh() const;

	std::vector<std::shared_ptr<Mesh>> getMeshes() const;

	size_t getNumOfVertices() const;

private:
	std::vector<std::shared_ptr<Mesh>> m_meshes;
};