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
	mesh->mNumFaces = vertices.size() / 4;
	mesh->mFaces = new aiFace[mesh->mNumFaces];

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
	int xRez = 10;
	int yRez = 10;

	// vertex generation
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	int vertexCount = xRez * yRez * 4;

	vertices.reserve(vertexCount * 5);

	for (unsigned i = 0; i < yRez; i++)
	{
		for (unsigned j = 0; j < xRez; j++)
		{
			vertices.push_back((j / (float)xRez) - .5f);				// v.x
			vertices.push_back(0.0f);									// v.y
			vertices.push_back(i / (float)yRez - .5f);					// v.z
			vertices.push_back(j / (float)xRez);						// u
			vertices.push_back(i / (float)yRez);						// v

			vertices.push_back(((j + 1) / (float)xRez) - .5f);			// v.x
			vertices.push_back(0.0f);									// v.y
			vertices.push_back(i / (float)yRez - .5f);					// v.z
			vertices.push_back((j + 1) / (float)xRez);					// u
			vertices.push_back(i / (float)yRez);						// v

			vertices.push_back((j / (float)xRez) - .5f);				// v.x
			vertices.push_back(0.0f);									// v.y
			vertices.push_back((i + 1) / (float)yRez - .5f);			// v.z
			vertices.push_back((j) / (float)xRez);						// u
			vertices.push_back((i + 1) / (float)yRez);					// v

			vertices.push_back(((j + 1) / (float)xRez) - .5f);			// v.x
			vertices.push_back(0.0f);									// v.y
			vertices.push_back((i + 1) / (float)yRez - .5f);			// v.z
			vertices.push_back((j + 1) / (float)xRez);					// u
			vertices.push_back((i + 1) / (float)yRez);					// v

			// Indices for two triangles forming a quad
			unsigned int topLeft = i * yRez + j;
			unsigned int topRight = topLeft + 1;
			unsigned int bottomLeft = (i + 1) * yRez + j;
			unsigned int bottomRight = bottomLeft + 1;

			// First triangle of the quad
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);

			// Second triangle of the quad
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
