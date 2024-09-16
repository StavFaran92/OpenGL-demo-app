#include "MeshExporter.h"

#include "Engine.h"
#include "Context.h"
#include "ProjectAssetRegistry.h"

std::string MeshExporter::exportMesh(Resource<Mesh> mesh, const aiScene* scene)
{
	auto& projectDir = Engine::get()->getProjectDirectory();
	Assimp::Exporter exporter;
	const std::string savedFilePath = projectDir + "/" + mesh.getUID() + ".dae";
	exporter.Export(scene, "collada", savedFilePath, aiProcess_Triangulate);
	Engine::get()->getContext()->getProjectAssetRegistry()->addMesh(mesh.getUID());
	return savedFilePath;
}
