#include "Terrain.h"
#include "Texture.h"
#include "VertexLayout.h"
#include "Assets.h"
#include "MeshBuilder.h"
#include "Factory.h"
#include "MeshExporter.h"

aiScene* generateScene(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
{
	// Create a new mesh
	aiMesh* mesh = new aiMesh();
	mesh->mNumVertices = vertices.size();
	mesh->mVertices = new aiVector3D[mesh->mNumVertices];
	mesh->mNormals = new aiVector3D[mesh->mNumVertices];
	mesh->mTextureCoords[0] = new aiVector3D[mesh->mNumVertices];
	mesh->mNumUVComponents[0] = 2;

	// Set vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) 
	{
		mesh->mVertices[i] = aiVector3D(vertices[i * 5 + 0], vertices[i * 5 + 1], vertices[i * 5 + 2]);
		mesh->mTextureCoords[0][i] = aiVector3D(vertices[i * 5 + 3], vertices[i * 5 + 4], 0.0f);
	}

	mesh->mNumFaces = indices.size() / 3; // Number of triangles (3 indices per triangle)
	mesh->mFaces = new aiFace[mesh->mNumFaces];

	// Set faces
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
		mesh->mFaces[i].mNumIndices = 3;
		mesh->mFaces[i].mIndices = new unsigned int[3] { indices[i * 3 + 0], indices[i * 3 + 1], indices[i * 3 + 2] };
	}

	mesh->mPrimitiveTypes = aiPrimitiveType_TRIANGLE;

	// Create a new scene
	aiScene* scene = new aiScene();
	scene->mNumMeshes = 1;
	scene->mMeshes = new aiMesh * [1] { mesh };
	scene->mNumMaterials = 1;
	scene->mMaterials = new aiMaterial * [1] { new aiMaterial() };
	scene->mRootNode = new aiNode();
	scene->mRootNode->mNumMeshes = 1;
	scene->mRootNode->mMeshes = new unsigned int[1] { 0 };

	return scene;
}

Terrain Terrain::generateTerrain(int width, int height, float scale, const std::string& heightMapFilepath)
{
	auto heightMap = Engine::get()->getSubSystem<Assets>()->importTexture2D(heightMapFilepath);

	return generateTerrain(width, height, scale, heightMap);
}

Terrain Terrain::generateTerrain(int width, int height, float scale, Resource<Texture> heightMap)
{
	int xRez = 2;
	int yRez = 2;

	// vertex generation
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	int vertexCount = (xRez + 1) * (yRez + 1);

	//vertices.reserve(vertexCount * 5);

	// Vertex generation
	for (int i = 0; i <= yRez; i++) { // Include the last row
		for (int j = 0; j <= xRez; j++) { // Include the last column
			// Vertex positions
			float xPos = (j / (float)xRez) - 0.5f;  // Normalize to [-0.5, 0.5]
			float zPos = (i / (float)yRez) - 0.5f;  // Normalize to [-0.5, 0.5]
			float yPos = 0.0f;  // Flat terrain

			// Texture coordinates
			float u = j / (float)xRez;
			float v = i / (float)yRez;

			// Add position and texture coordinate to vertex list
			vertices.push_back(xPos);  // x
			vertices.push_back(yPos);  // y
			vertices.push_back(zPos);  // z
			vertices.push_back(u);     // u
			vertices.push_back(v);     // v
		}
	}

	// Index generation for the grid
	for (int i = 0; i < yRez; i++) {
		for (int j = 0; j < xRez; j++) {
			int topLeft = i * (xRez + 1) + j;         // Corrected calculation with (xRez + 1)
			int topRight = topLeft + 1;
			int bottomLeft = (i + 1) * (xRez + 1) + j;
			int bottomRight = bottomLeft + 1;

			// First triangle of the quad (top-left, bottom-left, top-right)
			indices.push_back(topLeft);
			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}


	Resource<Mesh> mesh = Factory<Mesh>::create();

	VertexLayout layout;
	layout.attribs.push_back(LayoutAttribute::Positions);
	layout.attribs.push_back(LayoutAttribute::Texcoords);
	layout.numOfVertices = vertexCount;
	layout.build();

	MeshBuilder::builder()
		.addRawVertices(vertices.data(), layout)
		.addIndices(indices)
		.build(mesh);

	aiScene* scene = generateScene(vertices, indices);

	MeshExporter::exportMesh(mesh, scene);

	Terrain terrain;
	terrain.m_heightmap = heightMap;
	terrain.m_scale = scale;
	terrain.m_width = width;
	terrain.m_height = height;
	terrain.m_mesh = mesh;

	return terrain; // todo fix
}

Resource<Mesh> Terrain::getMesh() const
{
	return m_mesh;
}

float Terrain::getScale() const
{
	return m_scale;
}

Resource<Texture> Terrain::getHeightmap() const
{
	return m_heightmap;
}

int Terrain::getWidth() const
{
	return m_width;
}

int Terrain::getHeight() const
{
	return m_height;
}
