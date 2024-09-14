#pragma once

#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Resource.h"
#include "Mesh.h"

class MeshExporter
{
public:
	static std::string exportMesh(Resource<Mesh> mesh, const aiScene* scene);
};