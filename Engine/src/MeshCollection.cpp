#include "MeshCollection.h"

void MeshCollection::addMesh(const std::shared_ptr<Mesh>& mesh)
{
	m_meshes.push_back(mesh);
}

std::shared_ptr<Mesh> MeshCollection::getPrimaryMesh() const
{
	assert(m_meshes.size() > 0);
	return *m_meshes.begin();
}

std::vector<std::shared_ptr<Mesh>> MeshCollection::getMeshes() const
{
	return m_meshes;
}
